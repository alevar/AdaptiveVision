#include "cameraDetect.h"

CameraDetect::CameraDetect(Mat* newFrame){
	frame = newFrame;
}

int CameraDetect::approximateDistance(){

}

void CameraDetect::setCameraDistance(int*){

}

void CameraDetect::setHSV(int*,int*,int*,int*,int*,int*){

}
void CameraDetect::setHSV(int*,int*){

}

void CameraDetect::setResolution(int* width,int* height){
	resolution = {width,hight};
}

float CameraDetect::getDistance(){
	return results[0];
}
float CameraDetect::getLargestContour(){
	return results[1];
}

void CameraDetect::noiseReduction(){
	erode(imageTHR, imageTHR, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	dilate(imageTHR, imageTHR, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));

	dilate(imageTHR, imageTHR, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	erode(imageTHR, imageTHR, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
}
void CameraDetect::findLargestContour(){

}