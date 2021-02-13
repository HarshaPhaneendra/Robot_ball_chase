#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>
#include "std_msgs/String.h"

ros::ServiceClient client;

// function to drive robot via service
void drive_robot(float lin_x, float ang_z)
{
    ROS_INFO("Service requested..");

    // requesting wheel velocities 
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z; 

    if(!client.call(srv))
        ROS_INFO("Failed to call service 'command_robot'! ");

}

// calback funtion 
void process_image_callback(const sensor_msgs::Image img)
{
    ROS_INFO("process_image_callback() is started ");

    int white_pixel = 255;  
    int ball_pos;
    int ball_pos_center;
    int ball_pos_sum = 0;
    int white_pixel_num = 0;

    // searching for white pixel in image frame
    for (int k = 0; k+2 < img.data.size(); k = k+3)
    {
        if ((img.data[k] == white_pixel) && (img.data[k+1] == white_pixel) && (img.data[k+2] == white_pixel))
        {
            ball_pos = (k % (img.width * 3)) / 3;
            ball_pos_sum += ball_pos;
            white_pixel_num++;
        }
    }

    if (white_pixel_num == 0)
    {
        ROS_INFO("White pixel not found!, STOP the robot.");
        drive_robot(0,0);
    }
    else
    {
        ROS_INFO("White pixel found...");
        ball_pos_center = ball_pos_sum / white_pixel_num;

        if (ball_pos_center < img.width / 3)
        {
            ROS_INFO("Turn left");
            drive_robot(0.1, 0.1);
        }
        else if(ball_pos_center > img.width * 2 / 3)
        {
            ROS_INFO("Turn right");
            drive_robot(0.1, -0.1);
        }
        else
        {
            ROS_INFO("Go Straight");
            drive_robot(0.1, 0);
        }
    }

    ROS_INFO("End of process_image_callback()...");
    
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    client = n.serviceClient<ball_chaser::DriveToTarget>("ball_chaser/command_robot");

    ros::Subscriber sub_1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    ros::spin();
    return 0;
}