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

		CameraDetect(Mat*, Mat*, VideoCapture, VideoCapture); // Class object

		void setCameraDistance(float); // Allows the user to define the distance between two cameras

		float getCameraDistance();		// Allows the user to get the set distance values

		void setHSV(int,int,int,int,int,int);	// Allows the user to set all the HSV preferences
		void setHSV(int,int);					// Allows the user to only manipulate the Hue preferences
		vector<int> getHSV();					// Allows the user to get the currently set HSV values

		void setResolution(int,int);			// Allows the user to set same resolution for both cameras
		void setResolution(int,int,int,int);	// Allows the user to set different resolution to cameras
		vector<int> getResolution();			// allows the user to get the current resolution settings

	private:

/*=========================================================
============ 	 Global Private Variables    ==============
=========================================================*/

		Mat *frame1;	// the primary object passed in the initialization of the class
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

/*=========================================================
============ 	  RGB to HSV Conversion 	 ==============
=========================================================*/

		void convertRGB2HSV(Mat*, Mat*);

		Mat *imageHSV1;
		Mat *imageHSV2;

/*=========================================================
============ 	  Thresholding the Image	 ==============
=========================================================*/

		void thresholdHSV(Mat*, Mat*);

		Mat *imageTHR1;
		Mat *imageTHR2;

/*=========================================================
============ 	 	 Noise Reduction 		 ==============
=========================================================*/

		void noiseReduction(Mat*, Mat*);

		Mat *imageReduced1;
		Mat *imageReduced2;

/*=========================================================
============ 	Largest Contour Detection	 ==============
=========================================================*/

		void findLargestContour();

		Point2f largestCenter;
		float largestRadius;
		vector<float> largestContour;
		int largest_contour_index;
		double largest_area;

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		Mat canny_output;

};

#endif