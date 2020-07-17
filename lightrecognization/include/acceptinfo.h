#ifndef ACCEPT_INFO_H
#define ACCEPT_INFO_H
#include <cstdlib>
#include<geometry_msgs/PoseArray.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>
#include <unistd.h>
#include <yoloconfig.h>
static const int INPUT_H = Yolo::INPUT_H;
static const int INPUT_W = Yolo::INPUT_W;
float data[3 * INPUT_H * INPUT_W];
class  AcceptInfo{
    //private:
    
    public:
        ros::NodeHandle n;                            //b)
        ros::Publisher chatter_pub ;
        std::string base_frame_;
        std::string  map_frame_;
        tf::TransformListener tf_;
        static geometry_msgs::PoseArray turningZone_;
        //geometry_msgs::PoseArray turningZone_;
        AcceptInfo();
        bool getRobotGlobalPose(tf::Stamped<tf::Pose> &odom_pose);//call the function to get the location
        void turningZoneCallback(geometry_msgs::PoseArrayConstPtr msg);
        bool CheckInTurningZone();

};
geometry_msgs::PoseArray AcceptInfo::turningZone_;

AcceptInfo::AcceptInfo(){
    //base_frame_ = "golfcart/base_link";
    base_frame_ = YoloConfig::base_frame;
    //map_frame_ = "golfcart/map";
    map_frame_ = YoloConfig::map_frame;
    chatter_pub = n.advertise<std_msgs::String>("message", 1);
}
bool AcceptInfo::getRobotGlobalPose(tf::Stamped<tf::Pose> &odom_pose)//call the function to get the location
{       
    tf::Stamped<tf::Pose> robot_pose;
    
    robot_pose.setIdentity();
    robot_pose.frame_id_ = base_frame_;
    //robot_pose.frame_id_ = "golfcart/map";
    robot_pose.stamp_ = ros::Time(0);

    try
    {
        this->tf_.waitForTransform(map_frame_, base_frame_, robot_pose.stamp_, ros::Duration(0.5));
        this->tf_.transformPose(map_frame_, robot_pose, odom_pose);
    }
    catch (tf::LookupException &ex)
    {
        ROS_ERROR("No Transform available Error: %s\n", ex.what());
        return false;
    }
    catch (tf::ConnectivityException &ex)
    {
        ROS_ERROR("Connectivity Error: %s\n", ex.what());
        return false;
    }
    catch (tf::ExtrapolationException &ex)
    {
        ROS_ERROR("Extrapolation Error: %s\n", ex.what());
        return false;
    }
    return true;
}


void AcceptInfo::turningZoneCallback(geometry_msgs::PoseArrayConstPtr msg)
{       
    
    //geometry_msgs turningZone::Pose;
    //geometry_msgs::PoseArray turningZone_;
    turningZone_.poses = msg->poses;
    std::cout<<"Received !!!"<<std::endl;
    //LightRecognization::turningZone_
} 


bool AcceptInfo::CheckInTurningZone()//  turing zone  area
{   
    bool inturningzone_tmp;
    tf::Stamped<tf::Pose> robot_pose;
    getRobotGlobalPose(robot_pose);
    double robot_x = robot_pose.getOrigin().x();
    double robot_y = robot_pose.getOrigin().y();
    std::cout<<"robot_x:"<<robot_x<<std::endl;
    std::cout<<"robot_y:"<<robot_y<<std::endl;
    std::cout<<"turningZone_.poses[i].position.x"<<turningZone_.poses[0].position.x<<std::endl;
    std::cout<<"turningZone_.poses[i].position.y"<<turningZone_.poses[0].position.y<<std::endl;
    for (unsigned int i = 0; i < turningZone_.poses.size(); i++)
    {   
        double dist = pow((robot_x - turningZone_.poses[i].position.x) * (robot_x - turningZone_.poses[i].position.x) +
                            (robot_y - turningZone_.poses[i].position.y) * (robot_y - turningZone_.poses[i].position.y),
                        0.5);
        std::cout<<"dist:"<<dist<<std::endl;
        std::cout<<"turningZone_.poses[i].position.z:"<<turningZone_.poses[i].position.z<<std::endl;

        if (dist < turningZone_.poses[i].position.z)//will not be used
        {
            //turningzone_front_len_ = turningZone_.poses[i].orientation.x;
            //turningzone_rear_len_ = turningZone_.poses[i].orientation.y;
            //turningzone_max_vel_ = turningZone_.poses[i].orientation.z;

            inturningzone_tmp = true;
            break;
        }
        else
            inturningzone_tmp = false;
    }
    bool inTurningZone_ = inturningzone_tmp;

    return inTurningZone_;
}



#endif 
