#include <ros/ros.h>
#include "std_msgs/String.h"
#include "apple_pos.h"
#include <serial/serial.h>  //ROS已经内置了的串口包 
#include <iostream>

using namespace std;
#define random(x) (rand()%x)

object_locate::apple_pos apple_pos_msg;

serial::Serial ser; //声明串口对象 
int pos_buf[5];
//string o_pos;
char temp[10];
size_t flag;
//void chatterCallback(const std_msgs::String::ConstPtr& msg)
void chatterCallback(const object_locate::apple_pos& msg)
{
    pos_buf[0] = 4000;
    if(msg.pose.position.x<50&&msg.pose.position.y<250&&msg.pose.position.z<300
	&&msg.pose.position.x>10&&msg.pose.position.y>10&&msg.pose.position.z>30
	){
    	pos_buf[1] = msg.pose.position.x;
    	pos_buf[2] = msg.pose.position.y; 
    	pos_buf[3] = msg.pose.position.z; 
    }else{
       pos_buf[1] = 0;
       pos_buf[2] = 280-random(10)*10;
       pos_buf[3] = 200;	
    }
    pos_buf[4] = 1000;
    //sprintf(temp, "%d%d%d%d%d", pos_buf[0],pos_buf[1],pos_buf[2],pos_buf[3],pos_buf[4]);
    sprintf(temp, "%d%c%d%c%d%c%d%c%d", pos_buf[0],' ',pos_buf[1],' ',pos_buf[2],' ',pos_buf[3],' ',pos_buf[4]);
    ROS_INFO("Writing to serial port[%s]",temp) ;
    ser.write(temp);   //发送串口数据 
    //ROS_INFO("Writing to serial port[%d,%d,%d,%d,%d]",pos_buf[0],pos_buf[1],pos_buf[2],pos_buf[3],pos_buf[4]);
    // o_pos = temp;
    //printf("%d",flag);
    //ROS_INFO("flag[%s]",flag);
}
int main(int argc, char **argv)
{
    ros::init(argc, argv, "robot_arm_listener");
    ros::NodeHandle n;
    ros::Subscriber write_sub = n.subscribe("robot_pos_topic", 1000, chatterCallback);
    ros::Publisher read_pub = n.advertise<object_locate::apple_pos>("Arm_topic", 1000); 
    try 
    { 
    //设置串口属性，并打开串口 
        ser.setPort("/dev/ttyUSB0"); 
	//ser.setPort("/dev/pts/19"); 
        ser.setBaudrate(115200); 
        serial::Timeout to = serial::Timeout::simpleTimeout(10); 
        ser.setTimeout(to); 
        ser.open(); 
    } 
    catch (serial::IOException& e) 
    { 
        ROS_ERROR_STREAM("Unable to open port "); 
        return -1; 
    } 

    //检测串口是否已经打开，并给出提示信息 
    if(ser.isOpen()) 
    { 
        ROS_INFO_STREAM("Serial Port initialized"); 
    } 
    else 
    { 
        return -1; 
    } 
    //指定循环的频率,频率不能太高，一般2、3以下，否则机械臂无法控制 
    ros::Rate loop_rate(0.5); 
    while(ros::ok()) 
    { 
        if(ser.available()){ 
            ROS_INFO_STREAM("Reading from serial port\n"); 
            std_msgs::String result;
            result.data = ser.read(ser.available());
            ROS_INFO_STREAM("Read: "<< result.data);
	    //apple_pos_msg.data[0].data = 'A';
	    //apple_pos_msg.data[1].data = 'R';
            read_pub.publish(apple_pos_msg); 
        } 
        //处理ROS的信息，比如订阅消息,并调用回调函数 
        ros::spinOnce(); 
        loop_rate.sleep(); 
    } 
    return 0;
}
