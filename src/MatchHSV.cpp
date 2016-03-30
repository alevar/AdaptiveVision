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
#include "Sample.h"

using namespace std;
using namespace cv;

MatchHSV::MatchHSV(){
	MatchHSV::setThreshold(100,255);
}

MatchHSV::MatchHSV(Mat *image){
	this->inputIMG = image;

	// this->convertRGB2HSV();
	// this->thresholdHSV();
	// this->noiseReduction();
	// this->findAllContours();
	// this->findLargestContour();

}

MatchHSV::MatchHSV(Mat *image, Sample *sample){
	this->inputIMG = image;
	this->sample = sample;
	// this->convertRGB2HSV();
	// this->thresholdHSV();
	// this->noiseReduction();
	// this->findAllContours();
	// this->findLargestContour();

}

void MatchHSV::setHSV(int *lowHue,int *highHue,int *lowSat,int *highSat,int *lowVal,int *highVal){
	this->hsv.lowH = *lowHue;
	this->hsv.highH = *highHue;
	this->hsv.lowS = *lowSat;
	this->hsv.highS = *highSat;
	this->hsv.lowV = *lowVal;
	this->hsv.highV = *highVal;
	// *sample.setHSV(lowHue, highHue, lowSat, highSat, lowVal, highVal);

}

void MatchHSV::setHSV(int *lowHue,int *highHue){
	this->hsv.lowH = *lowHue;
	this->hsv.highH = *highHue;
	// *sample.setHSV( lowHue, highHue);
}

void MatchHSV::convertRGB2HSV(){

	cvtColor(inputORIG, inputORIG, COLOR_BGR2HSV);
	cvtColor(inputORIG, inputORIG, COLOR_BGR2HSV);
}

void MatchHSV::thresholdHSV(){

	if(!firstThreshold){
		namedWindow("HELLO MATCH HSV", 1);
		createTrackbar("Low HUE", "HELLO MATCH HSV", &this->hsv.lowH, 255);
		createTrackbar("High HUE", "HELLO MATCH HSV", &this->hsv.highH, 255);
		createTrackbar("Low SAT", "HELLO MATCH HSV", &this->hsv.lowS, 255);
		createTrackbar("High SAT", "HELLO MATCH HSV", &this->hsv.highS, 255);
		createTrackbar("Low VAL", "HELLO MATCH HSV", &this->hsv.lowV, 255);
		createTrackbar("High VAL", "HELLO MATCH HSV", &this->hsv.highV, 255);

		Mat testIMG = inputORIG;
		Mat test2IMG;

		while(true){

			inRange(testIMG, Scalar(this->hsv.lowH, this->hsv.lowS, this->hsv.lowV), Scalar(this->hsv.highH, this->hsv.highS, this->hsv.highV), test2IMG);

			imshow("HELLO MATCH HSV", test2IMG);
			if(waitKey(30) >= 0){
				inRange(inputORIG, Scalar(this->hsv.lowH, this->hsv.lowS, this->hsv.lowV), Scalar(this->hsv.highH, this->hsv.highS, this->hsv.highV), inputORIG);
				
				break;
			}
		}
		cvDestroyWindow("HELLO MATCH HSV");
		waitKey(1);
	    waitKey(1);
	    waitKey(1);
	    waitKey(1);
	    waitKey(1);
	    waitKey(1);
	    waitKey(1);
	    waitKey(1);

		cout << "VALUES ARE: " << hsv.lowH << endl;

		firstThreshold = true;
	}

	else{
		inRange(inputORIG, Scalar(this->hsv.lowH, this->hsv.lowS, this->hsv.lowV), Scalar(this->hsv.highH, this->hsv.highS, this->hsv.highV), inputORIG);
	}

}

void MatchHSV::noiseReduction(){
	
	erode(inputORIG, inputORIG, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	dilate(inputORIG, inputORIG, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	dilate(inputORIG, inputORIG, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	erode(inputORIG, inputORIG, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));

}

void MatchHSV::findAllContours(){

	Canny(inputORIG, canny_output, thresh.minThresh, thresh.minThresh*2, 3 );

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

void MatchHSV::setThreshold(int newMinThresh, int newMaxThresh){
	this->thresh.minThresh = newMinThresh;
	this->thresh.maxThresh = newMaxThresh;
}

void MatchHSV::compute(){
	inputORIG = inputIMG->clone();
	inputCOPY = inputIMG->clone();
	MatchHSV::convertRGB2HSV();
	MatchHSV::thresholdHSV();
	MatchHSV::noiseReduction();
	MatchHSV::findAllContours();
	MatchHSV::findLargestContour();
}

Point2f MatchHSV::getCoordinates(){
	return largestCenter;
}

void MatchHSV::extractSample(){
	this->tenPercent = int((largestRadius*20)/100);
	roi.x = (int(this->largestCenter.x-this->largestRadius-int(tenPercent/2)));
	roi.y = (int(this->largestCenter.y-this->largestRadius-int(tenPercent/2)));
	roi.width = (int(this->largestRadius*2)+tenPercent);
	roi.height = (int(this->largestRadius*2)+tenPercent);
	this->image_roi = this->inputCOPY(roi);
}

Mat MatchHSV::getSampleMAT(){
	return image_roi;
}

string MatchHSV::getSampleSTR(){
	return "hi";
}

void MatchHSV::show(){

	imshow("SAMPLE",image_roi);
	waitKey(0);
}

MatchHSV::~MatchHSV() {
	
}