#ifndef LIGHT_RECOGNIZATION_H
#define LIGHT_RECOGNIZATION_H
#include "ros/ros.h"
#include <dirent.h>
#include "logging.h"
#include "yololayer.h"
#include <std_msgs/String.h>
#include <acceptinfo.h>
#include <util.h>
#include <yolov3.h>
#include <camerainfo.h>
#include<yoloconfig.h>
using namespace std;

REGISTER_TENSORRT_PLUGIN(YoloPluginCreator);
class LightRecognization{
    public:
    
        LightRecognization(int argc,char **argv);


};

LightRecognization::LightRecognization(int argc,char **argv){

    cudaSetDevice(DEVICE);
    home_path = getenv("HOME");
    char *trtModelStream{nullptr};
    size_t size{0};
    sequence = 0;
    //if (!Util::isFileExists(home_path+"/yolo_data/yolov3.engine")) {  
    if (!Util::isFileExists(YoloConfig::yolov3_engine_path)) {  
        //if(!Util::isFileExists(home_path+"/yolo_data/")){
        if(!Util::isFileExists(YoloConfig::yolo_data_path)){
            	char *str = "mkdir -p ";
				//std::string const& cmd = std::string(str) + std::string(home_path+"/yolo_data/");
                std::string const& cmd = std::string(str) + std::string(YoloConfig::yolo_data_path);
				char const *command = cmd.c_str();
				
				char re=system(command);
        }
        IHostMemory* modelStream{nullptr};
        
        Yolov3::APIToModel(1, &modelStream);
        
        assert(modelStream != nullptr);
        //std::ofstream p(home_path+"/yolo_data/yolov3.engine", std::ios::binary);
        std::ofstream p(YoloConfig::yolov3_engine_path, std::ios::binary);
        if (!p) {
            std::cerr << "could not open plan output file" << std::endl;
            return ;
        }
        p.write(reinterpret_cast<const char*>(modelStream->data()), modelStream->size());
        modelStream->destroy();
 
    } 
    //std::ifstream file(home_path+"/yolo_data/yolov3.engine", std::ios::binary);
    std::ifstream file(YoloConfig::yolov3_engine_path, std::ios::binary);
    if (file.good()) {
        file.seekg(0, file.end);
        size = file.tellg();
        file.seekg(0, file.beg);
        trtModelStream = new char[size];
        assert(trtModelStream);
        file.read(trtModelStream, size);
        file.close();
    }
    //static float prob[OUTPUT_SIZE];
    IRuntime* runtime = createInferRuntime(gLogger);
    assert(runtime != nullptr);
    ICudaEngine* engine = runtime->deserializeCudaEngine(trtModelStream, size);
    assert(engine != nullptr);
    context = engine->createExecutionContext();
    assert(context != nullptr);

}

#endif 