#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "utils.h"
#include "cuda.h"
#include "blas.h"
#include "connected_layer.h"

#include<ros/ros.h> //ros标准库头文件
#include<iostream> //C++标准输入输出库
/*cv_bridge中包含CvBridge库*/
#include<cv_bridge/cv_bridge.h> 
/*ROS图象类型的编码函数*/
#include<sensor_msgs/image_encodings.h> 
/*  image_transport 头文件用来在ROS系统中的话题上发布和订阅图象消息*/
#include<image_transport/image_transport.h> 

#include <vector>
#include <iostream>
//OpenCV2标准头文件
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/objdetect/objdetect.hpp>
std::string face_cascade_name_std;
static const std::string INPUT = "Input"; //定义输入窗口名称
static const std::string OUTPUT = "Output"; //定义输出窗口名称

using namespace std;
using namespace cv;
cv::String face_cascade_name ;
//定义一个转换的类

//void apple_detector(char *datacfg, char *cfgfile, char *weightfile, IplImage* src, float thresh, int dont_show)

CascadeClassifier faceDetector; 

class RGB_GRAY
{
private:
    ros::NodeHandle nh_; //定义ROS句柄
    image_transport::ImageTransport it_; //定义一个image_transport实例
    image_transport::Subscriber image_sub_, image_sub_depth; //定义ROS图象接收器
    //image_transport::Publisher image_pub_; //定义ROS图象发布器
    cv::Point pt;
    double dis;
public:
    RGB_GRAY()
      :it_(nh_) //构造函数
    {
        image_sub_ = it_.subscribe("/camera/color/image_raw", 1, &RGB_GRAY::convert_callback, this); //定义图象接受器，订阅话题是“camera/rgb/image_raw”
	image_sub_depth = it_.subscribe("/camera/depth/image_raw",1,&RGB_GRAY::convert_callback_depth, this);
       // image_pub_ = it_.publishe("", 1); //定义图象发布器
        //初始化输入输出窗口
        //cv::namedWindow(INPUT);
	if (!nh_.getParam("face_cascade_name", face_cascade_name_std))
	      	ROS_ERROR("Could not get face_cascade_name");
    	face_cascade_name = face_cascade_name_std;
        cv:namedWindow(INPUT);
        cv::namedWindow(OUTPUT);
    }
    ~RGB_GRAY() //析构函数
    {
         cv::destroyWindow(INPUT);
         cv::destroyWindow(OUTPUT);
    }
    /*  这是一个ROS和OpenCV的格式转换回调函数，将图象格式从sensor_msgs/Image  --->  cv::Mat*/
    void convert_callback(const sensor_msgs::ImageConstPtr& msg) 
    {
        cv_bridge::CvImagePtr cv_ptr; // 声明一个CvImage指针的实例

        try
        {
            cv_ptr =  cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::RGB8); //将ROS消息中的图象信息提取，生成新cv类型的图象，复制给CvImage指针
        }
        catch(cv_bridge::Exception& e)  //异常处理
        {
            ROS_ERROR("cv_bridge exception: %s", e.what());
            return;
        }

	pt = color_locate(cv_ptr->image); //得到了cv::Mat类型的图象，在CvImage指针的image中，将结果传送给处理函数   
    }
    void convert_callback_depth(const sensor_msgs::ImageConstPtr& msg) 
    {
        cv_bridge::CvImagePtr cv_ptr; // 声明一个CvImage指针的实例

        try
        {
            cv_ptr =  cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::RGB8); //将ROS消息中的图象信息提取，生成新cv类型的图象，复制给CvImage指针
        }
        catch(cv_bridge::Exception& e)  //异常处理
        {
            ROS_ERROR("cv_bridge exception: %s", e.what());
            return;
        }
        uchar* data = cv_ptr->image.ptr<uchar>(pt.y);
	imshow(INPUT,cv_ptr->image);
	int div = 64;
	data[pt.x] =  data[pt.x]/div*div + div/2;
        dis = (double)data[pt.x];
	ROS_INFO("%s",dis);	 
    }

    /*这是图象处理的主要函数，一般会把图像处理的主要程序写在这个函数中。这里的例子只是一个彩色图象到灰度图象的转化 */

    Point color_locate(cv::Mat img) 
    {
        //友好错误信息提示  
     	 try{  
       		faceDetector.load(face_cascade_name);  
    	 } catch (cv::Exception e){}  
    	 if (faceDetector.empty())  
	 {  
       		std::cerr << "脸部检测器不能加载 (";  
		std::cerr << face_cascade_name << ")!" << std::endl;  
		exit(1); 
	 }
         cv::Mat img_out;
         cv::cvtColor(img, img_out, CV_RGB2GRAY);  //转换成灰度图象
	 Mat equalizedImg;  
         equalizeHist(img_out, equalizedImg);  
        //人 检测用Cascade Classifier::detectMultiScale来进行人脸检测  
         int flags = CASCADE_FIND_BIGGEST_OBJECT|CASCADE_DO_ROUGH_SEARCH;    //只检测脸最大的人  
         //int flags = CASCADE_SCALE_IMAGE;  //检测多个人  
         Size minFeatureSize(30, 30);  
         float searchScaleFactor = 1.1f;  
         int minNeighbors = 4;  
         std::vector<Rect> faces;  
         faceDetector.detectMultiScale(equalizedImg, faces, searchScaleFactor, minNeighbors, flags, minFeatureSize);  
         //画矩形框  
         cv::Mat face;  
         cv::Point text_lb;  
         for (size_t i = 0; i < faces.size(); i++)  
         {  
            if (faces[i].height > 0 && faces[i].width > 0)  
            {  
                face = img_out(faces[i]);  
                text_lb = cv::Point(faces[i].x, faces[i].y);  
                cv::rectangle(equalizedImg, faces[i], cv::Scalar(255, 0, 0), 1, 8, 0);  
		return text_lb;
            }  
         }  
	 
         cv::imshow(OUTPUT, equalizedImg);
         cv::waitKey(5);
    }
};

//主函数
int main(int argc, char** argv)
{
    ros::init(argc, argv, "RGB");
    RGB_GRAY obj;
    IplImage* src = cvLoadImage("apple.jpg",1);

    apple_detector("cfg/coco.names","cfg/yolo.cfg","yolo.weights",src,"-thresh 0.8","dont_show 1");
    ros::spin();
}

