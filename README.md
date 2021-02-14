# Robot_ball_chase 
`Robot_ball_chase` project is a part of Udacity Nano degree program "Robotics Engineering" deployed in ROS framework, it consists of two packages `my_robot` and `ball_chaser`. To achive ball chasing robot given that ball is of white color, algorithm has detect and locateed the white ball later robot is driven accordingly to the direction of white ball. If the white ball is out of frame/not found, robot is stoped. 

## my_robot 
`my_robot` consists of Gazebo simulated world and a customized two wheeled robot.
* Launch - To launch bunch of nodes in an orderly manner along with parameters.
* Meshes - Consists of `Hoyuko` Lidar sensor binary file.
* URDF files - A two wheeled robot is defined along with camera and Lidar sensors are attached.
* World - Consists of `Gazebo` simulated office environment world.

## ball_chaser
`ball_chaser` of two node, 
* process_image - It is responsible for analysing data frames of robot's front camera to detect and locate white ball. Later request for robot movements.
* drive_bot - Robot is driven according to the request made by previous node.


