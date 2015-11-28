#include "cameraDetect.h"

CameraDetect::CameraDetect(Mat* newFrame1, Mat* newFrame2, VideoCapture newSource1, VideoCapture newSource2){
	frame1 = newFrame1;
	frame2 = newFrame2;

	source1 = newSource1;
	source2 = newSource2;

	CameraDetect::setHSV(35,60,50,255,50,255);
	CameraDetect::setResolution(640,480);
}

void CameraDetect::setCameraDistance(float newDistanceBetweenCameras){
	cameraDistance = newDistanceBetweenCameras;
}

float CameraDetect::getCameraDistance(){
	return cameraDistance;
}

void CameraDetect::setHSV(int lowHue,int highHue,int lowSat,int highSat,int lowVal,int highVal){
	lowH = lowHue;
	highH = highHue;
	lowS = lowSat;
	highS = highSat;
	lowV = lowVal;
	highV = highVal;
}

void CameraDetect::setHSV(int lowHue,int highHue){
	lowH = lowHue;
	highH = highHue;
}

vector<int> CameraDetect::getHSV(){
	fullHSV = {lowH,highH,lowS,highS,lowV,highV};
	return fullHSV;
}

void CameraDetect::setResolution(int width, int height){
	source1.set(CV_CAP_PROP_FRAME_WIDTH,width);
	source1.set(CV_CAP_PROP_FRAME_HEIGHT,height);

	source2.set(CV_CAP_PROP_FRAME_WIDTH,width);
	source2.set(CV_CAP_PROP_FRAME_HEIGHT,height);

	resolution = {width, height};
	center_frame = {width/2, height/2};
}

vector<int> CameraDetect::getResolution(){
	return resolution;
}

