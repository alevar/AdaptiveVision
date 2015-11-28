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

		CameraDetect(Mat*); // int 1 => camera source,

		int approximateDistance();

		void setCameraDistance(int*);

		void setHSV(int*,int*,int*,int*,int*,int*);
		void setHSV(int*,int*);

		void setResolution(int*,int*);

		float getDistance();
		float getLargestContour();

	private:

		void noiseReduction();
		void findLargestContour();
		void convertRGB2HSV();
		void thresholdHSV();

		float results[5];

		Point2f largestCenter;
		float largestRadius;
		int largest_contour_index = 0;
		double largest_area = 0;

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		Mat canny_output;

		Mat frame;
		Mat imageHSV;
		Mat imageTHR;

		int thresh = 100;		// Minimum thresholding value for edge detection
		int max_thresh = 255;	// Maximum thresholding value for edge detection

		int lowH = 35;			//Low pass value for Hue
		int highH = 60;			//High pass value for Hue
		int lowS = 50;			//Low pass value for Saturation
		int highS = 255;		//High pass value for Saturation
		int lowV = 50;			//Low pass value for Value
		int highV = 255;		//High pass value for Value

		float *cameraDistance;
		float distance_object;

		int resolution[2] = {640,480};	 // The resolution of the camera
		int center_frame[2] = {320,240}; // The (x,y) coordinates of the center of the frame with the resolution 640*480

};

#endif