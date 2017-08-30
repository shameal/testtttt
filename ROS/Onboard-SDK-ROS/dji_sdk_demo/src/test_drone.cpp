#include <ros/ros.h>
#include <stdio.h>
#include <dji_sdk/dji_drone.h>
#include <cstdlib>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include "std_msgs/Int32.h"
#include <math.h>
using namespace DJI::onboardSDK;


int main(int argc, char **argv)
{
    ros::init(argc, argv, "sdk_client");
    ROS_INFO("sdk_service_client_test");
    ros::NodeHandle nh;
    DJIDrone* drone = new DJIDrone(nh);
    ros::Rate loop_rate(30);


    sleep(1);
	 while(1)
    {
	ros::spinOnce();
         if(drone->completeflag==1)
	{ 
        switch(drone->updateflight)
        {
	    
            case 0:
                /*taking  off*/
            drone->completeflag=0;
			drone->initial_motion.twist.angular.x=drone->motion.twist.angular.x;
        	drone->initial_motion.twist.angular.y=drone->motion.twist.angular.y;
        	drone->ideal_yaw=0;
			std::cout<<"Please tell me the incresement of x1: ";
        	std::cin>>drone->inc_x_1;
        	std::cout<<"Please tell me the incresement of y1: ";
        	std::cin>>drone->inc_y_1;
            std::cout<<"Please tell me the incresement of x2: ";
        	std::cin>>drone->inc_x_2;
        	std::cout<<"Please tell me the incresement of y2: ";
        	std::cin>>drone->inc_y_2;
            drone->ideal_motion.twist.angular.x=drone->initial_motion.twist.angular.x+drone->inc_x_1;
        	drone->ideal_motion.twist.angular.y=drone->initial_motion.twist.angular.y+drone->inc_y_1;
			drone->ideal_motion.twist.angular.z=1.3;
            std::cout<<"The ideal x is: "<<drone->initial_motion.twist.angular.x+drone->inc_x_1<<std::endl;
            std::cout<<"The ideal y is: "<<drone->initial_motion.twist.angular.y+drone->inc_y_1<<std::endl;
            drone->request_sdk_permission_control();
            sleep(5);
            drone->takeoff();
            sleep(6);
            drone->flightmode=1;//flightmode: pid with yaw and height control
            break;	 
            //landing
            case 1:
            	drone->completeflag=0;
               drone->landing();
               sleep(6);
               drone->completeflag=1;
               drone->updateflight = 0;
               break;
        }
    }


 }
}
