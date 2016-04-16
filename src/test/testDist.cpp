// g++ -std=c++11 -o testDist testDist.cpp `pkg-config opencv --cflags --libs`

#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc_c.h"

#include <vector>
#include <iostream>
#include <stdio.h>
#include <cmath>

#define PI 3.14159265

using namespace cv;
using namespace std;

struct hsvStruct {
    int lowH = 85;           //Low pass value for Hue
    int highH = 133;         //High pass value for Hue
    int lowS = 74;          //Low pass value for Saturation
    int highS = 196;     //High pass value for Saturation
    int lowV = 143;          //Low pass value for Value
    int highV = 255;     //High pass value for Value
};

struct Thresh {
    int minThresh = 100;
    int maxThresh = 255;
};

bool threshSet = false;

int distCams = 19;
int viewAngle = 120;

double finalAngle1;
double finalAngle2;

double finalAngle1Rad;
double finalAngle2Rad;

int finalDist;

int main( int argc, char** argv ){

    hsvStruct hsv;
    Thresh thresh;

    Mat inputSCN1;
    Mat inputSCN2;

    Mat inputSCN1_THRESH;
    Mat inputSCN2_THRESH;

    vector<vector<Point> > contours1;
    vector<vector<Point> > contours2;
    vector<Vec4i> hierarchy1;
    vector<Vec4i> hierarchy2;
    Mat canny_output1;
    Mat canny_output2;

    Point2f largestCenter1;
    float largestRadius1;
    vector<float> largestContour1;
    int largest_contour_index1;
    double largest_area1;
    Point2f largestCenter2;
    float largestRadius2;
    vector<float> largestContour2;
    int largest_contour_index2;
    double largest_area2;

    VideoCapture cap1(1);
    VideoCapture cap2(2);
    if(!cap1.isOpened() || !cap2.isOpened()){
        return -1;
    }

    int frameHeight = 240;
    int frameWidth = 320;
    double ratio = (double)frameWidth/(double)viewAngle;

    cap1.set(CV_CAP_PROP_FRAME_WIDTH,frameWidth);
    cap1.set(CV_CAP_PROP_FRAME_HEIGHT,frameHeight);
    cap2.set(CV_CAP_PROP_FRAME_WIDTH,frameWidth);
    cap2.set(CV_CAP_PROP_FRAME_HEIGHT,frameHeight);

    namedWindow("CAM1",1);
    namedWindow("CAM2",1);

    namedWindow("HELLO MATCH HSV", 1);
    createTrackbar("Low HUE", "HELLO MATCH HSV", &hsv.lowH, 255);
    createTrackbar("High HUE", "HELLO MATCH HSV", &hsv.highH, 255);
    createTrackbar("Low SAT", "HELLO MATCH HSV", &hsv.lowS, 255);
    createTrackbar("High SAT", "HELLO MATCH HSV", &hsv.highS, 255);
    createTrackbar("Low VAL", "HELLO MATCH HSV", &hsv.lowV, 255);
    createTrackbar("High VAL", "HELLO MATCH HSV", &hsv.highV, 255);

    Mat inputTHRESH;
    Mat finalTHRESH;

    while(true){

        cap1 >> inputSCN1;
        cap2 >> inputSCN2;
      
        if(!threshSet){

            inputTHRESH = inputSCN2.clone();
            cvtColor(inputTHRESH, inputTHRESH, COLOR_BGR2HSV);
            

            // while(true){

                inRange(inputTHRESH, Scalar(hsv.lowH, hsv.lowS, hsv.lowV), Scalar(hsv.highH, hsv.highS, hsv.highV), finalTHRESH);

                imshow("HELLO MATCH HSV", finalTHRESH);
                if(waitKey(30) >= 0){
                    threshSet = true;
                    inRange(inputTHRESH, Scalar(hsv.lowH, hsv.lowS, hsv.lowV), Scalar(hsv.highH, hsv.highS, hsv.highV), inputTHRESH);
                    
                    // break;
                }
            // }
            // cvDestroyWindow("HELLO MATCH HSV");
            // waitKey(1);
            // waitKey(1);
            // waitKey(1);
            // waitKey(1);
            // waitKey(1);
            // waitKey(1);
            // waitKey(1);
            // waitKey(1);
            

        }
        else{

            cvtColor(inputSCN1, inputSCN1_THRESH, COLOR_BGR2HSV);
            cvtColor(inputSCN2, inputSCN2_THRESH, COLOR_BGR2HSV);

            inRange(inputSCN1_THRESH, Scalar(hsv.lowH, hsv.lowS, hsv.lowV), Scalar(hsv.highH, hsv.highS, hsv.highV), inputSCN1_THRESH);
            inRange(inputSCN2_THRESH, Scalar(hsv.lowH, hsv.lowS, hsv.lowV), Scalar(hsv.highH, hsv.highS, hsv.highV), inputSCN2_THRESH);

            erode(inputSCN1_THRESH, inputSCN1_THRESH, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
            dilate(inputSCN1_THRESH, inputSCN1_THRESH, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
            dilate(inputSCN2_THRESH, inputSCN2_THRESH, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
            erode(inputSCN2_THRESH, inputSCN2_THRESH, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));


            Canny(inputSCN1_THRESH, canny_output1, thresh.minThresh, thresh.minThresh*2, 3 );
            findContours(canny_output1, contours1, hierarchy1, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
            Canny(inputSCN2_THRESH, canny_output2, thresh.minThresh, thresh.minThresh*2, 3 );
            findContours(canny_output2, contours2, hierarchy1, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

            vector<vector<Point>> contours_poly1(contours1.size());
            vector<Point2f> center1(contours1.size());
            vector<float> radius1(contours1.size());
            vector<vector<Point>> contours_poly2(contours2.size());
            vector<Point2f> center2(contours2.size());
            vector<float> radius2(contours2.size());

            if(contours1.size()>0){

                for( int i = 0; i < contours1.size(); i++ )
                {
                    double area1 = contourArea(contours1[i],false);
                    if(area1>largest_area1){
                        largest_area1 = area1;
                        largest_contour_index1 = i;
                    }
                    if(i == contours1.size()-1){
                        minEnclosingCircle( Mat (contours1[largest_contour_index1]), largestCenter1, largestRadius1); // Allows better estimation of the real size of the object, independent of the rotation
                        largestContour1 = {largestCenter1.x,largestCenter1.y, largestRadius1};
                    }
                }
            }

            if(contours2.size()>0){

                for( int i = 0; i < contours2.size(); i++ )
                {
                    double area2 = contourArea(contours2[i],false);
                    if(area2>largest_area2){
                        largest_area2 = area2;
                        largest_contour_index2 = i;
                    }
                    if(i == contours2.size()-1){
                        minEnclosingCircle( Mat (contours2[largest_contour_index2]), largestCenter2, largestRadius2); // Allows better estimation of the real size of the object, independent of the rotation
                        largestContour2 = {largestCenter2.x,largestCenter2.y, largestRadius2};
                    }
                }
            }

            Scalar color(132,132,132);

            circle( inputSCN1, Point2f(largestContour1[0],largestContour1[1]), (int)largestContour1[2], color, 2, 8, 0 );

            finalAngle1 = ((double)largestContour1[0]/ratio)-((double)viewAngle/2)+90;
            finalAngle2 = ((double)largestContour2[0]/ratio)-((double)viewAngle/2)+90;

            finalAngle1Rad = (finalAngle1*PI)/180;
            finalAngle2Rad = (finalAngle2*PI)/180;

            int testDist = (int)(20/(cos(finalAngle1Rad)/sin(finalAngle1Rad))+(cos(finalAngle2Rad)/sin(finalAngle2Rad)));

            finalDist = (int)((distCams*tan(finalAngle1Rad)*tan(finalAngle2Rad))/(tan(finalAngle1Rad)+tan(finalAngle2Rad)));

            cout << "finalDist: " << (finalDist/10)*10 << " TEST DIST: " << 0-(testDist/10)*10 << " POSITION1: " << finalAngle1Rad << " POSITION2: " << finalAngle2Rad << endl;

            imshow("HSV1",inputSCN1_THRESH);
            imshow("HSV2",inputSCN2_THRESH);
            imshow("CAM1",inputSCN1);
            imshow("CAM2",inputSCN2);
            if(waitKey(30) >= 0){
                break;
            }
        }

        
    }
    return 0;
}
