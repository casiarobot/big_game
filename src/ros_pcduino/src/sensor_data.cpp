//#include <Aduino.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <apple_pos.h>
#include <sstream>
#include <Arduino.h>
#define Sensor_m GPIO14
#define Sensor_l GPIO15
#define Sensor_r GPIO16
#define Sensor_b GPIO17

object_locate::apple_pos apple_pos_msg;

unsigned long duration;
char ID_Dis[4];
char buffer[16];
void setup(void){
// pinMode(4,OUTPUT);
//Serial.begin(9600);
}



int getDistance(unsigned int pin){
	pinMode(pin,OUTPUT);
	digitalWrite(pin,LOW);
	delayMicroseconds(2);
	digitalWrite(pin,HIGH);
	delayMicroseconds(5);
	digitalWrite(pin,LOW);

	pinMode(pin,INPUT);
	duration = pulseIn(pin,HIGH,30000);
	//printf("%d",duration);
	int distance = (int)duration/58.0;
//	sprintf(ID_Dis,"%d:%d",pin,distance);
	return distance;
}
void loop(void){
	int argc;
	char **argv;
	char temp[10];
	if(!argc)
		argc = 0;
	if(!argv)
		argv =NULL;
	//digitalWrite(4,HIGH);	
	ros::init(argc,argv,"talker");
	ros::NodeHandle n;
	ros::Publisher sonic_pub = n.advertise<std_msgs::String>("ultrasonic_topic",1000);
	ros::Rate loop_rate(1);
	
	while(ros::ok())
	{
		std_msgs::String msg;
		//msg.data = "Please put the ultrasonic data here!!!\n";/*Please put the ultrasonic data here!!!*/
	//	msg.data = getDistance(Sensor_l);
	//	memset(buffer,0,sizeof(buffer)/sizeof(char));
  		sprintf(buffer,"middile-%d,left-%d,right-%d,back-%d",getDistance(Sensor_m),getDistance(Sensor_l),getDistance(Sensor_r),getDistance(Sensor_b));
//		sprintf(buffer,"m-%d",getDistance(Sensor_m));
		msg.data= buffer;
		ROS_INFO("%s",msg.data.c_str());
		sonic_pub.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();
	}
	exit(0);
}
