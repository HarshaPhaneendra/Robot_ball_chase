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
    int white_pixel_number = { };
    bool white_pixel_found = false;
    /* enum split_image{left, middle, right}; 
    split_image side; */
    //int matrix_image[img.height][img.width] = { };


    //to find white pixel and store its pixel number
    for(int i=0; i < img.height*img.step; i++)
    {
        if(img.data[i] == white_pixel)
        {
            ROS_INFO("White pixel found!!");
            white_pixel_found = true;
            white_pixel_number = i;
            ROS_INFO_STREAM("white pixel number: " << std::to_string(white_pixel_number));
            break;
        }
    }

   
    // To find which side white pixel lie and drive robot accordingly 
    if(white_pixel_found == true)
    {
        int diciding_factor = white_pixel_number % img.width;
        ROS_INFO_STREAM("diciding factor: " << std::to_string(diciding_factor));
        ROS_INFO_STREAM("image width*2/3: " << std::to_string(img.width*2/3));
        if(diciding_factor < img.width/3 )
        {
            ROS_INFO("Image left");
            drive_robot(0.1,0.1);
            //ros::Duration(3).sleep();
        }
        else if( diciding_factor >= (img.width/3) && diciding_factor <= (img.width*2/3))
        {
            ROS_INFO("Image middel");
            drive_robot(0.5,0);
            //ros::Duration(3).sleep();

        }
        else if(diciding_factor > (img.width*2/3) )
        {
            ROS_INFO("Image right");
            drive_robot(0.1,-0.1);
            //ros::Duration(3).sleep();

        }
        else
        {
            ROS_INFO("couldn't find robot direction ");
        }
    }
    else
    {
        drive_robot(0,0);
        ros::Duration(3).sleep();
        ROS_INFO("No white pixel found!!, STOP the robot");
    }

    ROS_INFO("End of process_image_callback()...");

    /* for(int i=0; i < img.height*(img.step/3); i++)
    {
        if(img.data[i] == white_pixel)
        {
            side = left;
            ROS_INFO("White pixel found at left side of Image!!");
            break;
        }
    }

    for(int i=img.height*(img.step/3); i < img.height*(2*img.step/3); i++)
    {
        if(img.data[i] == white_pixel)
        {
            side = middle;
            ROS_INFO("White pixel found at middle of Image!!");
            break;
        }
    }

    for(int i=img.height*(2*img.step/3); i < img.height*img.step; i++)
    {
        if(img.data[i] == white_pixel)
        {
            side = right;
            ROS_INFO("White pixel found at right side of Image!!");
            break;
        }
    } */

    
    /* // converting given image into matrix format
    for(int i=0; i < img.height; i++) //traversing through image rows
    {
        for(int j=0; j < img.step; j++)// traversing through image columns
        {
            matrix_image[i][j] = img.data[(i*img.step) + j];
            /* if(img.data[(i*img.width) + j] == 255)
            {
                ROS_INFO("White pixel found!!");
                break;
            } 
            //  
        }
    }*/

    /* printf("Image height: %d", img.height);
    printf("Image width: %d", img.width);
    printf("Image step: %d", img.step); */

    /* ROS_INFO("Image is converted to matrix format");
            
    // to find white pixel in image
    for(int i=0; i < img.height; i++) //traversing through image rows
    {
        for(int j=0; j < img.step; j++)// traversing through image columns
        {
            if(matrix_image[i][j] == white_pixel)
            {
                white_pixel_found = true;
                ROS_INFO("White pixel found!!");

                if(j < (img.width/3))
                {
                    side = left;
                    ROS_INFO("White pixel found on left side of image");

                }
                else if((img.width/3) < j < 2*(img.width/3))
                {
                    side = middle;
                    ROS_INFO("White pixel found at middel of image");
                }    
                else
                {
                    side = right;
                    ROS_INFO("White pixel found on right side of image");
                }    
                               
                break;
            }
        }
    }  */

    // driving robot 
    /* if(white_pixel_found == true)
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
    {
        drive_robot(0,0);
        ROS_INFO("No white pixel found!!, STOP the robot"); 
    } */
        


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