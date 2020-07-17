//#ifndef LIGHT_RECOGNIZATION_H
//#define LIGHT_RECOGNIZATION_H
#include "ros/ros.h"
#include<yoloconfig.h>
#include<lightrecognization.h>
#include<yoloconfig.h>
int main(int argc,char **argv)
{
    std::cout<<YoloConfig::yolo_data_path<<endl;
    std::cout<<YoloConfig::yolov3_engine_path<<endl;
    std::cout<<YoloConfig::yolov3_reco_result_img<<endl;
    std::cout<<YoloConfig::yolov3_wts_path<<endl;

    ros::init(argc,argv,"lightrecognization");    //a)
     
    LightRecognization lr(argc,argv);
                                  //回调函数
    ros::NodeHandle n; 
    AcceptInfo lre;
    CameraInfo cio;
    
    ros::Subscriber sub11 = n.subscribe(YoloConfig::turning_zone_topic, 1, &AcceptInfo::turningZoneCallback,&lre);
    
    ros::Subscriber sub2 = n.subscribe(YoloConfig::img_raw_topic,1,&CameraInfo::imageCallback,&cio);    //c)
    
    ros::spin();
    return 0;
}

//#endif 