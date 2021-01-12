#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include "sensor_msgs/Image.h"

ros::ServiceClient client;

void drive_robot(float lin_x, float ang_z)
{
    ROS_INFO("Driving robot to target");

    // requesting wheel velocities 
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z; 

    if(!client.call(srv))
        ROS_INFO("Failed to call service 'command_robot'! ");

}

void process_image_callback(const sensor_msgs::Image img)
{
    int white_pixel = 255;
    bool white_pixel_found = false;
    enum split_image{left, middle, right}; 
    split_image side;
    int matrix_image[img.height][img.width] = { };
    
    // converting given image into matrix format
    for(int i=0; i < img.height; i++) //traversing through image rows
    {
        for(int j=0; j < img.width; j++)// traversing through image columns
        {

            matrix_image[i][j] = img.data[(i*img.height) + j];
        }
    }
            
    // to find white pixel in image
    for(int i=0; i < img.height; i++) //traversing through image rows
    {
        for(int j=0; j < img.width; j++)// traversing through image columns
        {
            if(matrix_image[i][j] == 255)
            {
                white_pixel_found = true;
                if(j < (img.width/3))
                   side = left;
                else if((img.width/3) < j < 2*(img.width/3))
                    side = middle;
                else    
                    side = right;
                
                break;
            }
        }
    }

    if(white_pixel_found == true)
    {
        switch (side)
        {
            case 0: ROS_INFO("Image left");
                    drive_robot(0,0.5);
                    break;

            case 1: ROS_INFO("Image middel");
                    drive_robot(0.5,0);
                    break;

            case 2: ROS_INFO("Image right");
                    drive_robot(0,-0.5);
                    break;

            default: ROS_INFO("You messed up!!");
                    break;

        }
    }
    else
        drive_robot(0,0);
        ROS_INFO("No white pixel found!!, STOP the robot");


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