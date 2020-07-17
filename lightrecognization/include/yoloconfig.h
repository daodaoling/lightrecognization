#ifndef YOLO_CONFIG_H
#define YOLO_CONFIG_H
#include<iostream>
using namespace std;
class YoloConfig{
    public:
        static std::string yolov3_wts_path;
        static std::string yolo_data_path;
        static std::string yolov3_engine_path;
        static std::string yolov3_reco_result_img;
        static std::string base_frame;
        static std::string map_frame;
        static std::string turning_zone_topic;
        static std::string img_raw_topic;
};
//to be modified
std::string YoloConfig::base_frame = "golfcart/base_link";
std::string YoloConfig::map_frame = "golfcart/map";
std::string YoloConfig::yolo_data_path = "/home/pengzhao/yolo_data/";
std::string YoloConfig::turning_zone_topic = "/golfcart/turningZone";
std::string YoloConfig::img_raw_topic = "/usb_cam/image_raw";

//do not modify it 
std::string YoloConfig::yolov3_wts_path = yolo_data_path + "yolov3.wts";
std::string YoloConfig::yolov3_engine_path = yolo_data_path + "yolov3.engine";
std::string YoloConfig::yolov3_reco_result_img = yolo_data_path + "/result/";

#endif