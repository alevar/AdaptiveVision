/*=====================================
============= CLASS SAMPLE ============
=======================================
==== Clas for creating images that ====
= are sent to the server for analysis =
=====================================*/

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

#include "Sample.h"

using namespace std;
using namespace cv;

Sample::Sample(){
	Sample::setThreshold(100,255);
}

Sample::Sample(Mat *image){
	this->inputCOPY = *image;
	this->inputIMG = image;
	this->convertRGB2HSV();
	this->thresholdHSV();
	this->noiseReduction();
	this->findAllContours();
	this->findLargestContour();
	this->extractSample();

}

void Sample::setHSV(int *lowHue,int *highHue,int *lowSat,int *highSat,int *lowVal,int *highVal){
	this->hsv.lowH = *lowHue;
	this->hsv.highH = *highHue;
	this->hsv.lowS = *lowSat;
	this->hsv.highS = *highSat;
	this->hsv.lowV = *lowVal;
	this->hsv.highV = *highVal;
}

void Sample::setHSV(int *lowHue,int *highHue){
	this->hsv.lowH = *lowHue;
	this->hsv.highH = *highHue;
}

void Sample::convertRGB2HSV(){

	cvtColor(*inputIMG, *inputIMG, COLOR_BGR2HSV);
	cvtColor(*inputIMG, *inputIMG, COLOR_BGR2HSV);
}

void Sample::thresholdHSV(){

	inRange(*inputIMG, Scalar(this->hsv.lowH, this->hsv.lowS, this->hsv.lowV), Scalar(this->hsv.highH, this->hsv.highS, this->hsv.highV), *inputIMG);

}

void Sample::noiseReduction(){
	
	erode(*inputIMG, *inputIMG, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	dilate(*inputIMG, *inputIMG, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	dilate(*inputIMG, *inputIMG, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	erode(*inputIMG, *inputIMG, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));

}

void Sample::findAllContours(){

	Canny(*inputIMG, canny_output, thresh.minThresh, thresh.minThresh*2, 3 );

	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
}

void Sample::findLargestContour(){

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

void Sample::setThreshold(int newMinThresh, int newMaxThresh){
	this->thresh.minThresh = newMinThresh;
	this->thresh.maxThresh = newMaxThresh;
}

void Sample::extractSample(){

	this->tenPercent = int((largestRadius*20)/100);
	roi.x = (int(this->largestCenter.x-this->largestRadius-int(tenPercent/2)));
	roi.y = (int(this->largestCenter.y-this->largestRadius-int(tenPercent/2)));
	roi.width = (int(this->largestRadius*2)+tenPercent);
	roi.height = (int(this->largestRadius*2)+tenPercent);
	this->image_roi = this->inputCOPY(roi);
}

Mat Sample::getSampleMAT(){
	return image_roi;
}

string Sample::getSampleSTR(){
	return "hi";
}

void Sample::show(){

	imshow("HELLO LARGEST CONTOUR",image_roi);
	if(waitKey(50) ==27){
		destroyWindow("HELLO LARGEST CONTOUR");
	}
}

Sample::~Sample() {
	
}