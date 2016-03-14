/*=====================================
============= CLASS SAMPLE ============
=======================================
==== Clas for creating images that ====
= are sent to the server for analysis =
=====================================*/

#ifndef SAMPLE_H
#define	SAMPLE_H

#include <string>
#include <vector>

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

using namespace std;
using namespace cv;

struct hsvStruct_SAMPLE {
	int lowH = 0;			//Low pass value for Hue
	int highH = 85;			//High pass value for Hue
	int lowS = 54;			//Low pass value for Saturation
	int highS = 126;		//High pass value for Saturation
	int lowV = 93;			//Low pass value for Value
	int highV = 218;		//High pass value for Value
};

struct Thresh_SAMPLE {
	int minThresh = 100;
	int maxThresh = 255;
};

struct ParamsHSV_SAMPLE {
	int center_frame[2] = {320,240};  // The (x,y) coordinates of the center of the frame with the resolution 640*480
	int radius_frame = 90;        	 // The minimum desired radius of the object being tracked
	int area_frame = 25447;         // The desired area of the object that is being tracked
	int radius_frame_max = 160;    // The maximum desired radius of the object being tracked
	int area_frame_max = 80425;   // The maximum desired area of the object being tracked
	int size[2] = {640,480};	 // The resolution of the camera
};

class Sample {

	public:

		Sample();
		Sample(Mat*);
		void setHSV(int*,int*,int*,int*,int*,int*);
		void setHSV(int*,int*);
		void setThreshold(int,int);
		void compute();
		Mat getSampleMAT();
		string getSampleSTR();
		void show();
		virtual ~Sample();	    
	
	private:
		Mat *inputIMG;
		Mat inputCOPY;
		Mat image_roi;

		Rect roi;

		hsvStruct_SAMPLE hsv;
		Thresh_SAMPLE thresh;
		ParamsHSV_SAMPLE params;

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		Mat canny_output;

		Point2f largestCenter;
		float largestRadius;
		vector<float> largestContour;
		int largest_contour_index;
		double largest_area;

		int tenPercent;

		void noiseReduction();
		void thresholdHSV();
		void convertRGB2HSV();
		void findAllContours();
		void findLargestContour();		
		void extractSample();

};

#endif	/* SAMPLE_H */

