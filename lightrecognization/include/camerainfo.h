#ifndef CAMERA_INFO_H
#define CAMERA_INFO_H
#include <std_msgs/String.h>
#include <sensor_msgs/Image.h>
#include <opencv2/core/core.hpp>
#include <cv_bridge/cv_bridge.h>
#include <util.h>
#include <yolov3.h>
#include <yoloconfig.h>
using namespace std;
static float prob[OUTPUT_SIZE];
IExecutionContext* context;
static int sequence;
class CameraInfo{
    public:
        void imageCallback(const sensor_msgs::ImageConstPtr &msg);

};

void CameraInfo::imageCallback(const sensor_msgs::ImageConstPtr &msg)
{
        //std::cout<<"enter img callback"<<endl;
        AcceptInfo *lre = new AcceptInfo();
        bool isInTurningZone = lre->CheckInTurningZone();
        if(!isInTurningZone){
            std::cout<<"Not in the zone !"<<endl;
            return ;       
        }
        std::cout<<"I am in the zone !"<<endl;

        
        imshow("img", cv_bridge::toCvShare(msg, "bgr8")->image);
        
        cv::Mat img =cv_bridge::toCvShare(msg, "bgr8")->image;
        
        cv::waitKey(1);
        
        cv::Mat pr_img = Util::preprocess_img(img);
        for (int i = 0; i < INPUT_H * INPUT_W; i++) {
            data[i] = pr_img.at<cv::Vec3b>(i)[2] / 255.0;
            data[i + INPUT_H * INPUT_W] = pr_img.at<cv::Vec3b>(i)[1] / 255.0;
            data[i + 2 * INPUT_H * INPUT_W] = pr_img.at<cv::Vec3b>(i)[0] / 255.0;
        }
        
        // Run inference
        auto start = std::chrono::system_clock::now();
       
        Yolov3::doInference(*context, data, prob, 1);
       
        auto end = std::chrono::system_clock::now();
        
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
        std::vector<Yolo::Detection> res;
        Util::nms(res, prob);
        for (int i=0; i<20; i++) {
            std::cout << prob[i] << ",";
        }
        std::cout<<"==";
        std::cout << res.size() << std::endl;
        for (size_t j = 0; j < res.size(); j++) {
            float *p = (float*)&res[j];
            for (size_t k = 0; k < 7; k++) {
                std::cout << p[k] << ", ";
            }
            std::cout << std::endl;
            cv::Rect r = Util::get_rect(img, res[j].bbox);
            if(&r !=NULL){
                std::cout<<"______________________________________________________________"<<endl;
                cv::rectangle(img, r, cv::Scalar(0x27, 0xC1, 0x36), 2);
                cv::putText(img, std::to_string((int)res[j].class_id), cv::Point(r.x, r.y - 1), cv::FONT_HERSHEY_PLAIN, 1.2, cv::Scalar(0xFF, 0xFF, 0xFF), 2);
                std_msgs::String msg;            //创建消息变量，变量类型必须符合发送的要求

                msg.data = std::to_string((int)res[j].class_id);
                lre->chatter_pub.publish(msg); 

            }

        }
        std::cout<<"==";
        //cv::imwrite("_" + f, img);dst
        
        //if(!Util::isFileExists(home_path+"/yolo_data/result/")){
        if(!Util::isFileExists(YoloConfig::yolov3_reco_result_img)){
            	char *str = "mkdir -p ";
				//std::string const& cmd = std::string(str) + std::string(home_path+"/yolo_data/result/");
                std::string const& cmd = std::string(str) + std::string(YoloConfig::yolov3_reco_result_img);
				char const *command = cmd.c_str();
				//std::cout<<c<<endl;
				char re=system(command);
        }
        //cv::imwrite(home_path+"/yolo_data/result/"+std::to_string(sequence)+".jpg" , img);
        cv::imwrite(YoloConfig::yolov3_reco_result_img+std::to_string(sequence)+".jpg" , img);
        
        
        std_msgs::String msg2;            //创建消息变量，变量类型必须符合发送的要求

        
        msg2.data = "2";
        
        std::cout<<msg2.data<<endl;
        lre->chatter_pub.publish(msg2); 
           
        sequence = sequence +1;

}

#endif 