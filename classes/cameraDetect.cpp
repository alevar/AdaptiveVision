#include "cameraDetect.h"

CameraDetect::CameraDetect(Mat* newFrame1, Mat* newFrame2, VideoCapture newSource1, VideoCapture newSource2){
	frame1 = newFrame1;
	frame2 = newFrame2;

	source1 = newSource1;
	source2 = newSource2;

	CameraDetect::setHSV(35,60,50,255,50,255);
	CameraDetect::setResolution(640,480);
	CameraDetect::setThreshold(100,255);

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

vector<Mat> CameraDetect::thresholdHSV(Mat image1, Mat image2){
	Mat imageTHROne;
	Mat imageTHRTwo;
	inRange(image1, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), imageTHROne);
	inRange(image2, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), imageTHRTwo);
	resultHSV2THR = {imageTHROne,imageTHRTwo};
	return resultHSV2THR;

}

vector<Mat> CameraDetect::convertRGB2HSV(Mat* image1, Mat* image2){
	Mat imageHSVOne;
	Mat imageHSVTwo;
	cvtColor(*image1, imageHSVOne, COLOR_BGR2HSV);
	cvtColor(*image2, imageHSVTwo, COLOR_BGR2HSV);
	resultRGB2HSV = {imageHSVOne,imageHSVTwo};
	return resultRGB2HSV;
}

void CameraDetect::setThreshold(int newMinThresh, int newMaxThresh){
	minThresh = newMinThresh;
	maxThresh = newMaxThresh;
}

vector<int> CameraDetect::getThreshold(){
	fullThreshold = {minThresh,maxThresh};
	return fullThreshold;
}

void CameraDetect::findAllContours(Mat* image){

	Canny(*image, canny_output, minThresh, minThresh*2, 3 );

	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
}

void CameraDetect::findLargestContour(vector<vector<Point> > newContours){

	vector<vector<Point>> contours_poly(newContours.size());
  	vector<Point2f> center(newContours.size());
  	vector<float> radius(newContours.size());

	if(newContours.size()>0){

		for( int i = 0; i < newContours.size(); i++ )
		{
			double area = contourArea(newContours[i],false);
			if(area>largest_area1){
				largest_area1 = area;
				largest_contour_index1 = i;
			}
			if(i == newContours.size()-1){
				minEnclosingCircle( Mat (newContours[largest_contour_index1]), largestCenter1, largestRadius1); // Allows better estimation of the real size of the object, independent of the rotation
				largestContour1 = {largestCenter1.x,largestCenter1.y, largestRadius1};
			}
		}
	}
}

vector<Mat> CameraDetect::Compute(){
	vector<Mat> resultHSV = CameraDetect::convertRGB2HSV(frame1,frame2);
	vector<Mat> resultTHR = CameraDetect::thresholdHSV(resultHSV[0],resultHSV[1]);
	cout<<resultTHR[1] << endl;

	return resultHSV;
}