/*ros interface*/
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <Arduino.h>
#include <apple_pos.h>

#define wheel_l GPIO3
#define wheel_l_d1 GPIO1
#define wheel_l_d2 GPIO2
#define wheel_r GPIO6
#define wheel_r_d1 GPIO4
#define wheel_r_d2 GPIO5
#define freq 781
//#define MAX_DUTY 250

char* name[20];

int value =  30;

int step_r,step_l;
object_locate::apple_pos apple_pos_msg;

struct sensor
{
	int m;
	int l;
	int r;
	int b;	
} sensors;


char buf[3];

void turnLeft(int velocity_l,unsigned int velocity_r){
	digitalWrite(wheel_r_d1,HIGH);
	digitalWrite(wheel_r_d2,LOW);
	if(velocity_l>0){
		digitalWrite(wheel_l_d1,HIGH);
		digitalWrite(wheel_l_d2,LOW);
	}else{
		digitalWrite(wheel_l_d1,LOW);
		digitalWrite(wheel_l_d2,HIGH);
	}
	step_l = pwmfreq_set(wheel_l,freq);
	step_r = pwmfreq_set(wheel_r,freq);
	//if(step_r>0) 
			analogWrite(wheel_r,velocity_l);
    //if(step_l>0)
			analogWrite(wheel_l,velocity_r);
}

void turnRight(unsigned int velocity_l,int velocity_r){

	digitalWrite(wheel_l_d1,HIGH);
	digitalWrite(wheel_l_d2,LOW);
	if(velocity_r>0){
		digitalWrite(wheel_r_d1,HIGH);
		digitalWrite(wheel_r_d2,LOW);
	}else{
		digitalWrite(wheel_r_d1,LOW);
		digitalWrite(wheel_r_d2,HIGH);
	}
    step_l =  pwmfreq_set(wheel_r,freq);
	//if(step_l>0)
			analogWrite(wheel_r,velocity_r);
	step_r =  pwmfreq_set(wheel_l,freq);
	//if(step_r>0)
			analogWrite(wheel_l,velocity_l);
}


void move_forward(unsigned int velocity_l,int velocity_r){

	digitalWrite(wheel_l_d1,HIGH);
	digitalWrite(wheel_l_d2,LOW);
	digitalWrite(wheel_r_d1,HIGH);
	digitalWrite(wheel_r_d2,LOW);

    step_l =  pwmfreq_set(wheel_r,freq);
	//if(step_l>0)
			analogWrite(wheel_r,velocity_r);
	step_r =  pwmfreq_set(wheel_l,freq);
	//if(step_r>0)
			analogWrite(wheel_l,velocity_l);
}


void move_back(unsigned int velocity_l,int velocity_r){

	digitalWrite(wheel_l_d1,LOW);
	digitalWrite(wheel_l_d2,HIGH);
	digitalWrite(wheel_r_d1,LOW);
	digitalWrite(wheel_r_d2,HIGH);

    step_l =  pwmfreq_set(wheel_r,freq);
	//if(step_l>0)
			analogWrite(wheel_r,velocity_r);
	step_r =  pwmfreq_set(wheel_l,freq);
	//if(step_r>0)
			analogWrite(wheel_l,velocity_l);
}
void stop()
{	
	digitalWrite(wheel_l_d1,LOW);
	digitalWrite(wheel_l_d2,LOW);
	digitalWrite(wheel_r_d1,LOW);
	digitalWrite(wheel_r_d2,LOW);
	
    step_l =  pwmfreq_set(wheel_r,0);
	
	step_r =  pwmfreq_set(wheel_l,0);
}
void setup(void)
{
	pinMode(wheel_l,OUTPUT);
	pinMode(wheel_l_d1,OUTPUT);
	pinMode(wheel_l_d2,OUTPUT);
	pinMode(wheel_r,OUTPUT);
	pinMode(wheel_r_d1,OUTPUT);
	pinMode(wheel_r_d2,OUTPUT);
}

void robot_pos_Callback(const object_locate::apple_pos& msg)
{

	ROS_INFO("I heard: [%d]",msg.data[0].data);
	/*base the position of the robot control the car to the apple tree location !!!*/
 		
//	turnLeft(781,781);
}

void sensor_distance_solve(char* temp)
{
	int i,k,x;
	for(i =0;temp[i];i++)
	{
		if(sscanf(temp+i,"%d%n",&k,&x))
			printf("%d",k);
		switch(i){
			case 0: sensors.m = k;break;
			case 1: sensors.l = k;break;
			case 2: sensors.r = k;break;
			case 3: sensors.b = k;break;	
		}
		i+=x;
	}
}

void sensor_Callback(const std_msgs::String::ConstPtr& msg)
{
	ROS_INFO("I heard: [%s]",msg->data.c_str());
	/*base the position of the robot control the car to the apple tree location !!!*/
//	strcpy(name,msg->data.c_str());
    
//	move_forward(250,250);
//	sleep(10);
	move_forward(250,250);
//	sleep(10);
//	move_back(125,125);
//	sleep(10);
//	stop();

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
	ros::Subscriber robot_pos_sub = n.subscribe("robot_pos_topic",1000,robot_pos_Callback);
	ros::Subscriber sensor_data_sub = n.subscribe<std_msgs::String>("ultrasonic_topic",1000,sensor_Callback);
	ros::spin();
}
