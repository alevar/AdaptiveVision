/*==================================================
==================== MATCH HSV CLASS ===============
====================================================
===== Class intended for identification of     =====
===== Matches between Templates and the input  =====
==================================================*/

#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <arpa/inet.h>
#include <stdlib.h>

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

#include "MatchHSV.h"

using namespace std;
using namespace cv;

MatchHSV::MatchHSV(){
	MatchHSV::setThreshold(100,255);
}

MatchHSV::MatchHSV(Mat *image){
	this->inputIMG = image;
	this->noiseReduction();
}

void MatchHSV::setHSV(int *lowHue,int *highHue,int *lowSat,int *highSat,int *lowVal,int *highVal){
	this->hsv.lowH = *lowHue;
	this->hsv.highH = *highHue;
	this->hsv.lowS = *lowSat;
	this->hsv.highS = *highSat;
	this->hsv.lowV = *lowVal;
	this->hsv.highV = *highVal;
}

void MatchHSV::setHSV(int *lowHue,int *highHue){
	this->hsv.lowH = *lowHue;
	this->hsv.highH = *highHue;
}

void MatchHSV::noiseReduction(){
	
	erode(*inputIMG, *inputIMG, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	dilate(*inputIMG, *inputIMG, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	dilate(*inputIMG, *inputIMG, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	erode(*inputIMG, *inputIMG, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));

	imshow("HELLO MATCH HSV", *inputIMG);
	waitKey(0);

}

void MatchHSV::thresholdHSV(){

	inRange(*inputIMG, Scalar(this->hsv.lowH, this->hsv.lowS, this->hsv.lowV), Scalar(this->hsv.highH, this->hsv.highS, this->hsv.highV), *inputIMG);

}

void MatchHSV::convertRGB2HSV(){

	cvtColor(*inputIMG, *inputIMG, COLOR_BGR2HSV);
	cvtColor(*inputIMG, *inputIMG, COLOR_BGR2HSV);
}

void MatchHSV::setThreshold(int newMinThresh, int newMaxThresh){
	this->thresh.minThresh = newMinThresh;
	this->thresh.maxThresh = newMaxThresh;
}

void MatchHSV::findAllContours(){

	Canny(*inputIMG, canny_output, thresh.minThresh, thresh.minThresh*2, 3 );

	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
}

void MatchHSV::findLargestContour(){

	vector<vector<Point>> contours_poly(contours.size());
  	vector<Point2f> center(contours.size());
  	vector<float> radius(contours.size());

	if(contours.size()>0){

		for( int i = 0; i < contours.size(); i++ )
		{
			double area = contourArea(contours[i],false);
			if(area>largest_area){
				largest_area = area;
				largest_contour_index = i;
			}
			if(i == contours.size()-1){
				minEnclosingCircle( Mat (contours[largest_contour_index]), largestCenter, largestRadius); // Allows better estimation of the real size of the object, independent of the rotation
				largestContour = {largestCenter.x,largestCenter.y, largestRadius};
			}
		}
	}
}

void MatchHSV::compute(){
	MatchHSV::convertRGB2HSV();
	MatchHSV::thresholdHSV();
	MatchHSV::noiseReduction();
	MatchHSV::findAllContours();
	MatchHSV::findLargestContour();
}

void MatchHSV::show(){
	
}

MatchHSV::~MatchHSV() {
	
}