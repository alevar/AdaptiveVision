// g++ -std=c++11 -o testHSV testHSV.cpp `pkg-config opencv --cflags --libs`

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int HueL = 23;
int SatL = 23;
int ValL = 23;
int HueH = 179;
int SatH = 179;
int ValH = 179;

int main( int argc, char** argv ){

    Mat inputSCN = imread("/home/sparrow/Pictures/Webcam/testSCN.png");
    cvtColor(inputSCN,inputSCN,COLOR_BGR2HSV);

    // VideoCapture cap(0);
    // if(!cap.isOpened()){
    //     return -1;
    // }

    namedWindow("MATCH",1);
    while(true){
      
        // cap >> inputSCN;

        namedWindow("HELLO MATCH HSV", 1);
        createTrackbar("Low HUE", "HELLO MATCH HSV", &HueL, 255);
        createTrackbar("High HUE", "HELLO MATCH HSV", &HueH, 255);
        createTrackbar("Low SAT", "HELLO MATCH HSV", &SatL, 255);
        createTrackbar("High SAT", "HELLO MATCH HSV", &SatH, 255);
        createTrackbar("Low VAL", "HELLO MATCH HSV", &ValL, 255);
        createTrackbar("High VAL", "HELLO MATCH HSV", &ValH, 255);

        Mat testIMG = inputSCN;
        Mat test2IMG;

        while(true){

            inRange(inputSCN, Scalar(HueL, SatL, ValL), Scalar(HueH,SatH,ValH), test2IMG);

            imshow("HELLO MATCH HSV", test2IMG);
            if(waitKey(30) >= 0){
                
                break;
            }
        }

        imshow("MATCH",inputSCN);
        if(waitKey(30) >= 0){
            break;
        }
    }
    return 0;
}
