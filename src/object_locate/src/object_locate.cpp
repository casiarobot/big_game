// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2015 Intel Corporation. All Rights Reserved.

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "utils.h"
#include "cuda.h"
#include "blas.h"
#include "connected_layer.h"


#include <librealsense/rs.hpp>
#include "example.hpp"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <thread>

#include<ros/ros.h> //ros标准库头文件
#include "std_msgs/String.h"
#include "geometry_msgs/Pose.h"
//#include<iostream> //C++标准输入输出库
/*cv_bridge中包含CvBridge库*/
#include<cv_bridge/cv_bridge.h> 
/*ROS图象类型的编码函数*/
#include<sensor_msgs/image_encodings.h> 
/*  image_transport 头文件用来在ROS系统中的话题上发布和订阅图象消息*/
#include<image_transport/image_transport.h> 

#include <vector>
//#include <iostream>
//OpenCV2标准头文件
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/objdetect/objdetect.hpp>

#include <object_locate/apple_pos.h>

std::string face_cascade_name_std;
static const std::string INPUT = "Input"; //定义输入窗口名称
static const std::string OUTPUT = "Output"; //定义输出窗口名称

using namespace std;
using namespace cv;
using namespace rs;
cv::String face_cascade_name ;
//定义一个转换的类

CascadeClassifier faceDetector; 

texture_buffer buffers[6];

#pragma pack(push, 1)
struct rgb_pixel
{
    uint8_t r,g,b; 
};

struct depth_params
{
      uint16_t *depthImage ;
      float scale;
      uint16_t depthValue;
      float depthInMeters;
} dp;

/*typedef struct {
	int h;
	int w;
	int c;
	float* data;
} image;*/



//extern void apple_detector(char *, char *, char *, IplImage*, float, int );
extern void apple_detector(char *datacfg, char *cfgfile, char *weightfile, image src, float thresh, int dont_show);
#pragma pack(pop)
Point color_locate(cv::Mat img);
Point pt;
//robot message 

geometry_msgs::Pose pos;
object_locate::apple_pos msg;
//std_msgs::String msg;

// Window size and frame rate
int const INPUT_WIDTH      = 640;
int const INPUT_HEIGHT     = 480;
int const FRAMERATE        = 30;
 
// Named windows
char* const WINDOW_DEPTH = "Depth Image";
char* const WINDOW_RGB     = "RGB Image";
 
 
context      _rs_ctx;
device&      _rs_camera = *_rs_ctx.get_device( 0 );
intrinsics   _depth_intrin;
intrinsics  _color_intrin;
bool         _loop = true;
/*
float getDistance(int x, int y){
    uint16_t *depthImage = (uint16_t *) dev->get_frame_data(rs::stream::depth);
    float scale = dev->get_depth_scale();
    rs::intrinsics depthIntrin = dev->get_stream_intrinsics(rs::stream::depth);
    uint16_t depthValue = depthImage[y * depthIntrin.width + x];
    float depthInMeters = depthValue * scale;
    return depthInMeters;
} 
*/


 
image ipl2image(IplImage* src)
{
    unsigned char *data = (unsigned char *)src->imageData;
    int h = src->height;
    int w = src->width;
    int c = src->nChannels;
    int step = src->widthStep;
    image out = make_image(w, h, c);
    int i, j, k, count=0;;

    for(k= 0; k < c; ++k){
        for(i = 0; i < h; ++i){
            for(j = 0; j < w; ++j){
                out.data[count++] = data[i*step + j*c + k]/255.;
            }
        }
    }
    return out;
}


// Initialize the application state. Upon success will return the static app_state vars address
 
bool initialize_streaming( )
{
       bool success = false;
       if( _rs_ctx.get_device_count( ) > 0 )
       {
             _rs_camera.enable_stream( rs::stream::color, INPUT_WIDTH, INPUT_HEIGHT, rs::format::rgb8, FRAMERATE );
             _rs_camera.enable_stream( rs::stream::depth, INPUT_WIDTH, INPUT_HEIGHT, rs::format::z16, FRAMERATE );
             _rs_camera.start( );
 
             success = true;
       }
       return success;
}
 
/////////////////////////////////////////////////////////////////////////////
// If the left mouse button was clicked on either image, stop streaming and close windows.
/////////////////////////////////////////////////////////////////////////////
static void onMouse( int event, int x, int y, int, void* window_name )
{
       if( event == cv::EVENT_LBUTTONDOWN )
       {
             _loop = false;
       }
}
 
 
/////////////////////////////////////////////////////////////////////////////
// Create the depth and RGB windows, set their mouse callbacks.
// Required if we want to create a window and have the ability to use it in
// different functions
/////////////////////////////////////////////////////////////////////////////
void setup_windows( )
{
       cv::namedWindow( WINDOW_DEPTH, 0 );
       cv::namedWindow( WINDOW_RGB, 0 );
 
       cv::setMouseCallback( WINDOW_DEPTH, onMouse, WINDOW_DEPTH );
       cv::setMouseCallback( WINDOW_RGB, onMouse, WINDOW_RGB );
}
 
 
/////////////////////////////////////////////////////////////////////////////
// Called every frame gets the data from streams and displays them using OpenCV.
/////////////////////////////////////////////////////////////////////////////
bool display_next_frame( )
{
 
       _depth_intrin       = _rs_camera.get_stream_intrinsics( rs::stream::depth );
       _color_intrin       = _rs_camera.get_stream_intrinsics( rs::stream::color );
        
 
       // Create depth image
       cv::Mat depth16( _depth_intrin.height,
                                  _depth_intrin.width,
                                  CV_16U,
                                  (uchar *)_rs_camera.get_frame_data( rs::stream::color_aligned_to_depth ) );
 
       // Create color image
       cv::Mat rgb( _color_intrin.height,
                            _color_intrin.width,
                            CV_8UC3,
                            (uchar *)_rs_camera.get_frame_data( rs::stream::color ) );
 
       // < 800
       cv::Mat depth8u = depth16;
       depth8u.convertTo( depth8u, CV_8UC1, 255.0/1000);
 	
      // imshow( WINDOW_DEPTH, depth8u );
      // cvWaitKey( 1 );
 
       cv::cvtColor( rgb, rgb, cv::COLOR_BGR2RGB );
       pt = color_locate(rgb);
	
       //calcute the disance of the app tree
       if(pt.x<1)
       {
	     pt.x = INPUT_WIDTH/2;
	     pt.y = INPUT_HEIGHT/2;
       }
       dp.depthImage = (uint16_t *) _rs_camera.get_frame_data(rs::stream::depth);
       dp.scale = _rs_camera.get_depth_scale();
       dp.depthValue = dp.depthImage[pt.y * _depth_intrin.width + pt.x];
       dp.depthInMeters = dp.depthValue * dp.scale;
       cvWaitKey( 1 );
       return true;
}


int main(int argc, char * argv[]) try
{
    ros::init(argc, argv, "RGB");
    ros::NodeHandle nh_;
    if (!nh_.getParam("face_cascade_name", face_cascade_name_std))
	      	ROS_ERROR("Could not get face_cascade_name");
    	face_cascade_name = face_cascade_name_std;
    pt.x = INPUT_WIDTH/2;
    pt.y = INPUT_HEIGHT/2;
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

    ros::Publisher chatter_pub = nh_.advertise<object_locate::apple_pos>("robot_pos_topic",1000); 
    ros::Rate loop_rate(10);

    rs::log_to_console(rs::log_severity::warn);
    //rs::log_to_file(rs::log_severity::debug, "librealsense.log");

    if( !initialize_streaming( ) )
    {
         std::cout << "Unable to locate camera!" << std::endl;
         rs::log_to_console( rs::log_severity::fatal );
         return EXIT_FAILURE;
    }
 
    setup_windows( );
    while(ros::ok())
    {
        if( _rs_camera.is_streaming( ) )
            _rs_camera.wait_for_frames( );
        
        display_next_frame( );
	pos.position.x = pt.x - INPUT_WIDTH/2;
	pos.position.y = pt.y - INPUT_HEIGHT/2;;
	pos.position.z = dp.depthInMeters;
        msg.pose = pos;
        //ROS_INFO("%d",msg.num);
        chatter_pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
    }
    _rs_camera.stop( );
    cv::destroyAllWindows( );
    return EXIT_SUCCESS;
}
catch( const rs::error & e )
{
       std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
       return EXIT_FAILURE;
}
catch( const std::exception & e )
{
       std::cerr << e.what() << std::endl;
       return EXIT_FAILURE;
}


Point color_locate(cv::Mat img) 
{

         cv::Mat img_out;
         cv::cvtColor(img, img_out, CV_RGB2GRAY);  //转换成灰度图象
	 Mat equalizedImg;  
         equalizeHist(img_out, equalizedImg);  
        //人 检测用Cascade Classifier::detectMultiScale来进行人脸检测  
         //int flags = CASCADE_FIND_BIGGEST_OBJECT|CASCADE_DO_ROUGH_SEARCH;    //只检测脸最大的人  
         int flags = CASCADE_SCALE_IMAGE;  //检测多个人  
         Size minFeatureSize(10, 10);  
         float searchScaleFactor = 1.1f;  
         int minNeighbors = 4;  
         std::vector<Rect> faces;  
         faceDetector.detectMultiScale(equalizedImg, faces, searchScaleFactor, minNeighbors, flags, minFeatureSize);  
         //画矩形框  
         cv::Mat face;  
         cv::Point text_lb;
	 cv::Point temp  = Point(INPUT_WIDTH/2,INPUT_HEIGHT/2);  
         for (size_t i = 0; i < faces.size(); i++)  
         {  
            if (faces[i].height > 0 && faces[i].width > 0)  
            {  
                face = img_out(faces[i]);  
                text_lb = cv::Point(faces[i].x, faces[i].y);  
                cv::rectangle(equalizedImg, faces[i], cv::Scalar(255, 255, 255), 1, 8, 0);  
	        return text_lb;
            }  
         }  
	 
         cv::imshow(WINDOW_RGB, equalizedImg);
         cv::waitKey(5);
}


