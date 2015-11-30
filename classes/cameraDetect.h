#ifndef CAMERADETECT_H
#define CAMERADETECT_H

#define LIBRARY_VERSION 1.0.0

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

#include <iostream>
#include <cmath>
#include <string.h>
#include <unistd.h> 
#include <stdio.h>

using namespace cv; // USING THE NAMESPACE FOR OPENCV OBJECTS
using namespace std;

class CameraDetect{
	public:

		CameraDetect(Mat*, Mat*, VideoCapture, VideoCapture);

		void setCameraDistance(float);

		float getCameraDistance();

		void setHSV(int,int,int,int,int,int);
		void setHSV(int,int);
		vector<int> getHSV();

		void setResolution(int,int);
		void setResolution(int,int,int,int);
		vector<int> getResolution();

		void setThreshold(int,int);
		vector<int> getThreshold();

		vector<Mat> Compute();

	private:

		void noiseReduction(Mat*, Mat*);
		vector<Mat> thresholdHSV(Mat, Mat);
		vector<Mat> convertRGB2HSV(Mat*, Mat*);
		void findLargestContour();
		void findAllContours(Mat*);
		void findLargestContour(vector<vector<Point> >);

		Mat *frame1;
		Mat *frame2;

		VideoCapture source1;
		VideoCapture source2;

		float cameraDistance;

		vector<int> fullHSV;

		int lowH, highH, lowS, highS ,lowV, highV;

		vector<int> resolution;
		vector<int> resolutionDiff;
		vector<int> center_frame;
		bool diffResolution;

		vector<Mat> resultRGB2HSV;
		vector<Mat> resultHSV2THR;

		Mat *imageTHR1;
		Mat *imageTHR2;

		Mat *imageReduced1;
		Mat *imageReduced2;

		vector<int> fullThreshold;
		int minThresh;
		int maxThresh;

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		Mat canny_output;

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
};

#endif