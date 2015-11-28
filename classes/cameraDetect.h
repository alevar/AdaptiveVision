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
		vector<int> getResolution();

	private:

		void noiseReduction(Mat*, Mat*);
		void thresholdHSV(Mat*, Mat*);

		Mat *frame1;
		Mat *frame2;

		VideoCapture source1;
		VideoCapture source2;

		float cameraDistance;

		vector<int> fullHSV;

		int lowH, highH, lowS, highS ,lowV, highV;

		vector<int> resolution;
		vector<int> center_frame;

		Mat *imageTHR1;
		Mat *imageTHR2;

		Mat *imageReduced1;
		Mat *imageReduced2;

};

#endif