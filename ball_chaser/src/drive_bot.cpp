#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"
/* #include "std_msgs/Float64.h"
#include "std_msgs/String.h" */

//publisher object name 
ros::Publisher motor_command_publisher;

bool handle_drive_request(ball_chaser::DriveToTarget::Request& req, ball_chaser::DriveToTarget::Response& res)
{
    //std::vector<double> robot_vel =(req.linear_x, req.angular_z); 
/*     std_msgs::string response_msg; */

    geometry_msgs::Twist motor_command;
    motor_command.linear.x = req.linear_x;
    motor_command.angular.z = req.angular_z;

    //publishing angles to drive robot
    motor_command_publisher.publish(motor_command);

    //wait for 3seconds so that robot would drive to requested pos
    ros::Duration(3).sleep();

    //send response msg_feedback
    res.msg_feedback = "Wheel velocities set - linear_x:" + std::to_string(req.linear_x) + " angular_z:" + std::to_string(req.angular_z);
    ROS_INFO_STREAM(res.msg_feedback);

    return true;

}

int main(int argc, char** argv)
{
    ros::init (argc, argv, "drive_bot");

    ros::NodeHandle n; 

    // publisher data type and topic name
    motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    //creating a service 
    ros::ServiceServer sev = n.advertiseService("/ball_chaser/command_robot", handle_drive_request);
    ROS_INFO("Ready to send robot wheel velocities");
      

    ros::spin();

    return 0;
}