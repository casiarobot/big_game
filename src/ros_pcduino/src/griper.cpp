/*ros interface*/
#include "ros/ros.h"
#include "std_msgs/Char.h"
#include <apple_pos.h>
#include <Arduino.h>
#define SensorCut GPIO8
#define MotorCut GPIO6
#define SensorGrasp GPIO10
#define MotorGrasp GPIO11
//object_locate::apple_pos apple_pos_msg;
//char flag_buf = 'A';

char buf[3];
void servopulse(int servopin,int myangle)
{
	int pulsewidth = (myangle*11)+500;
	digitalWrite(servopin,HIGH);
	delayMicroseconds(pulsewidth);
	digitalWrite(servopin,LOW);
	delay(20 -pulsewidth/1000);
}

void  chatterCallback(const object_locate::apple_pos& msg)
{
	//ROS_INFO("I heard: [%s]",msg.data);
	/*base the position of the robot control the car to the apple tree location !!!*/
	buf[0] = msg.data[0].data;
	buf[1] = msg.data[1].data;
	//if("AR" == buf){
	int val =90;
	//val = val*(180/9);
	for(int i =0;i<=50;i++){
	
		servopulse(MotorGrasp,val);
		servopulse(MotorCut,val);
	}

	//pwmfreq_set(MotorGrasp,50);
    //pwmfreq_set(MotorCut,50);
	//analogWrite(MotorGrasp,20);
	//analogWrite(MotorCut,20);	
//	if(LOW==digitalRead(SensorCut))
			//	pwmfreq_set(MotorCut,0);
			//analogWrite(MotorCut,100);
//		if(LOW==digitalRead(SensorGrasp))
			//	pwmfreq_set(MotorGrasp,0);
	//}	
}

void setup(void)
{
	pinMode(SensorCut,INPUT);
	pinMode(MotorCut,OUTPUT);
	pinMode(SensorGrasp,INPUT);
	pinMode(MotorGrasp,OUTPUT);	
}
void loop(void){
    int argc;
	char **argv;
	if(!argc)
		argc = 0;
	if(!argv)
		argv =NULL;
	ros::init(argc,argv,"lisener");
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("robot_pos_topic",1000,chatterCallback);
	ros::spin();
}
