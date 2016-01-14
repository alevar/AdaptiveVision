#include "cameraDetect.h"

CameraDetect::CameraDetect(Mat* newFrame1, Mat* newFrame2, VideoCapture newSource1, VideoCapture newSource2){
	frame1 = newFrame1;
	frame2 = newFrame2;

	source1 = newSource1;
	source2 = newSource2;

	CameraDetect::setHSV(35,60,50,255,50,255);
	CameraDetect::setResolution(640,480);
	// CameraDetect::setThreshold(100,255);

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

// void CameraDetect::setThreshold(int newMinThresh, int newMaxThresh){
// 	minThresh = newMinThresh;
// 	maxThresh = newMaxThresh;
// }

// vector<int> CameraDetect::getThreshold(){
// 	fullThreshold = {minThresh,maxThresh};
// 	return fullThreshold;
// }

void CameraDetect::noiseReduction(Mat image1, Mat image2){
	Mat imageReducedOne;
	Mat imageReducedTwo
	erode(image1, imageReducedOne, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	dilate(image1, imageReducedOne, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	dilate(image1, imageReducedOne, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	erode(image1, imageReducedOne, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));

	erode(image2, imageReducedTwo, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	dilate(image2, imageReducedTwo, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	dilate(image2, imageReducedTwo, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	erode(image2, imageReducedTwo, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
}

void CameraDetect::thresholdHSV(Mat* image1, Mat* image2){
	inRange(*image1, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), *imageTHR1);
	inRange(*image2, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), *imageTHR2);
}

void CameraDetect::convertRGB2HSV(Mat* image1, Mat* image2){
	cvtColor(*image1, *imageHSV1, COLOR_BGR2HSV);
	cvtColor(*image2, *imageHSV2, COLOR_BGR2HSV);
}

// void CameraDetect::findContours(Mat* image1, Mat* image2){

// 	Canny(image1, canny_output, thresh, thresh*2, 3 );

// 	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
// }

// void CameraDetect::findLargestContour(Mat* image1, Mat* image2){

// 	vector<vector<Point>> contours_poly(contours.size());
//   	vector<Point2f> center(contours.size());
//   	vector<float> radius(contours.size());

// 	if(contours.size()>0){

// 		for( int i = 0; i < contours.size(); i++ )
// 		{
// 			double area = contourArea(contours[i],false);
// 			if(area>largest_area){
// 				largest_area = area;
// 				largest_contour_index = i;
// 			}
// 			if(i == contours.size()-1){
// 				minEnclosingCircle( Mat (contours[largest_contour_index]), largestCenter, largestRadius); // Allows better estimation of the real size of the object, independent of the rotation
// 				largestContour = {largestCenter,largestRadius};
// 			}
// 		}
// 	}
// }