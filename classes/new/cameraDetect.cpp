#include "cameraDetect.h"

CameraDetect::CameraDetect(Mat* newFrame1, Mat* newFrame2){
	frame1 = *newFrame1;
	frame2 = *newFrame2
}

int CameraDetect::approximateDistance(){
	cvtColor(frame, imageHSV, COLOR_BGR2HSV);
	
	inRange(imageHSV, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), imageTHR);

	noiseReduction();

	Canny(imageTHR, canny_output, thresh, thresh*2, 3 );

	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

	//distance_object = first_centerx - second_centerx;
	//results = {distance,largestCenter}

	return 0;
}

void CameraDetect::setCameraDistance(float* newDistance){
	cameraDistance = newDistance;

}

void CameraDetect::setHSV(int* lowHue,int* highHue,int* lowSat,int* highSat,int* lowVal,int* highVal){
	int lowH = *lowHue;
	int highH = *highHue;
	int lowS = *lowSat;
	int highS = *highSat;
	int lowV = *lowVal;
	int highV = *highVal;
}

void CameraDetect::setHSV(int* lowHue,int* highHue){
	lowH = *lowHue;
	highH = *highHue;
}

void CameraDetect::setResolution(int* width,int* height){
	resolution = {height, width};
	center_frame = {height/2, width/2};
}

float CameraDetect::getDistance(){
	return results[0];
}

float CameraDetect::getLargestContour(){
	return largestContour;
}

/*==================================================
================= Private Functions ================
==================================================*/

Mat CameraDetect::noiseReduction(){
	erode(imageTHR, imageTHR, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	dilate(imageTHR, imageTHR, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));

	dilate(imageTHR, imageTHR, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	erode(imageTHR, imageTHR, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
}

void CameraDetect::findLargestContour(){

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
				largestContour = {largestCenter,largestRadius};
			}
		}
	}
}

void CameraDetect::convertRGB2HSV(){
	cvtColor(frame, imageHSV, COLOR_BGR2HSV);
}

void CameraDetect::thresholdHSV(){
	inRange(imageHSV, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), imageTHR);
}