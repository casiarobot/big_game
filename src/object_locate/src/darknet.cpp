#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "utils.h"
#include "cuda.h"
#include "blas.h"
#include "connected_layer.h"

#include "opencv2/opencv.hpp"
using namespace cv;

//void apple_detector(char *datacfg, char *cfgfile, char *weightfile, IplImage* src, float thresh, int dont_show)
int main(int argc, char **argv)
{

    if(argc < 2){
        fprintf(stderr, "usage: %s <function>\n", argv[0]);
        return 0;
    }
    IplImage* src = cvLoadImage("apple.jpg",1);

    apple_detector("cfg/coco.names","cfg/yolo.cfg","yolo.weights",src,"-thresh 0.8","dont_show 1");
}

