#include "cameraDetect.h"

CameraDetect::CameraDetect(Mat* newFrame1, Mat* newFrame2, VideoCapture newSource1, VideoCapture newSource2){
	frame1 = newFrame1;
	frame2 = newFrame2;

	source1 = newSource1;
	source2 = newSource2;

	CameraDetect::setHSV(35,60,50,255,50,255);
	CameraDetect::setResolution(640,480);

	diffResolution = false;
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
	diffResolution = false;

	source1.set(CV_CAP_PROP_FRAME_WIDTH,width);
	source1.set(CV_CAP_PROP_FRAME_HEIGHT,height);

	source2.set(CV_CAP_PROP_FRAME_WIDTH,width);
	source2.set(CV_CAP_PROP_FRAME_HEIGHT,height);

	resolution = {width, height};
	center_frame = {width/2, height/2};
}

void CameraDetect::setResolution(int width1, int height1, int width2, int height2){
	diffResolution = true;

	source1.set(CV_CAP_PROP_FRAME_WIDTH,width1);
	source1.set(CV_CAP_PROP_FRAME_HEIGHT,height1);

	source2.set(CV_CAP_PROP_FRAME_WIDTH,width2);
	source2.set(CV_CAP_PROP_FRAME_HEIGHT,height2);

	resolutionDiff = {width1, height1, width2, height2};
	center_frame = {width1/2, height1/2, width2/2, height2/2};
}

vector<int> CameraDetect::getResolution(){
	if(diffResolution){
		return resolutionDiff;
	}
	else{
		return resolution;
	}
}

void CameraDetect::noiseReduction(Mat* image1, Mat* image2){
	imageReduced1 = image1;
	imageReduced2 = image2;
	erode(*imageReduced1, *imageReduced1, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	dilate(*imageReduced1, *imageReduced1, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	dilate(*imageReduced1, *imageReduced1, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	erode(*imageReduced1, *imageReduced1, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));

	erode(*imageReduced2, *imageReduced2, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	dilate(*imageReduced2, *imageReduced2, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	dilate(*imageReduced2, *imageReduced2, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	erode(*imageReduced2, *imageReduced2, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
}

void CameraDetect::thresholdHSV(Mat* image1, Mat* image2){
	inRange(*image1, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), *imageTHR1);
	inRange(*image2, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), *imageTHR2);
}

void CameraDetect::convertRGB2HSV(Mat* image1, Mat* image2){
	cvtColor(*image1, *imageHSV1, COLOR_BGR2HSV);
	cvtColor(*image2, *imageHSV2, COLOR_BGR2HSV);
}