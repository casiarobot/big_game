#include <ros/ros.h>
#include <object_locate/apple_pos.h>
#include <std_msgs/String.h>
#include <stdio.h>
#include <sys/time.h>  
#include <linux/input.h>  
#include <stdlib.h>  
#include <fcntl.h>
#include <iostream>

using namespace std;

void usage(char *str)  
{  
    fprintf(stderr, "<usage> %s /dev/input/eventX/n", str);  
    exit(1);  
}  
int main(int argc,char**argv)
{
    ros::init(argc,argv,"send");
    ros::NodeHandle n;
    //ros::Publisher pub  = n.advertise<object_locate::apple_pos>("robot_pos_topic",1000);
    ros::Publisher pub  = n.advertise<std_msgs::String>("robot_pos_topic",1000);
    ros::Rate loop_rate(10);
    int keys_fd;  

    char ret[2];  
    struct input_event t;  

    keys_fd = open ("/dev/input/event5", 0);  
    if (keys_fd <= 0)  
    {  
        printf ("open /dev/input/event5 device error!\n");  
        return 0;  
    }  

    while (ros::ok())  
    {     
	 std_msgs::String msg;  
   	 std::stringstream ss;  
  	 // ss << " I am the example1_a node ";  
   	 //msg.data = ss.str();  
   	 //ROS_INFO("%s", msg.data.c_str());  
   	 //object_locate::apple_pos apple_pos_msg;

	 if (read (keys_fd, &t, sizeof (t)) == sizeof (t))  
          { 
         	 if (t.type == EV_KEY) 
            	 if (t.value == 0 || t.value == 1)  
		 {  
			//printf ("key %d %s\n", t.code, (t.value) ? "Pressed" : "Released"); 
			ss<<"key:"<<t.code<<" "<<(t.value) ? "Pressed" : "Released";
			msg.data = ss.str();
			pub.publish(msg);         		
			if(t.code==KEY_ESC)   break;  
      		 }  
          }  
    }  
   ros::spinOnce();  
   loop_rate.sleep(); 
   close (keys_fd);  

  return 0;   
}


