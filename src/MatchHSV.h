/*==================================================
==================== HSV MATCH CLASS ===============
====================================================
===== Class intended for creation of Templates =====
===== Templates are used against the incoming  =====
===== images, to identify similar objects	   =====
==================================================*/

#ifndef MATCHHSV_H
#define	MATCHHSV_H

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

#include "Sample.h"

using namespace std;
using namespace cv;

struct hsvStruct {
	int lowH = 66;			//Low pass value for Hue
	int highH = 114;			//High pass value for Hue
	int lowS = 101;			//Low pass value for Saturation
	int highS = 201;		//High pass value for Saturation
	int lowV = 105;			//Low pass value for Value
	int highV = 173;		//High pass value for Value
};

struct Thresh {
	int minThresh = 100;
	int maxThresh = 255;
};

struct ParamsHSV {
	int center_frame[2] = {320,240};  // The (x,y) coordinates of the center of the frame with the resolution 640*480
	int radius_frame = 90;        	 // The minimum desired radius of the object being tracked
	int area_frame = 25447;         // The desired area of the object that is being tracked
	int radius_frame_max = 160;    // The maximum desired radius of the object being tracked
	int area_frame_max = 80425;   // The maximum desired area of the object being tracked
	int size[2] = {640,480};	 // The resolution of the camera
};

class MatchHSV {

	public:

		MatchHSV();
		MatchHSV(Mat*);
		MatchHSV(Mat*, Sample*);
		void setHSV(int*,int*,int*,int*,int*,int*);
		void setHSV(int,int,int,int,int,int);
		void setHSV(int*,int*);
		void setThreshold(int,int);
		void compute();
		Point2f getCoordinates();				
		void extractSample();
		Mat getSampleMAT();
		string getSampleSTR();
		void show();
		virtual ~MatchHSV();	    
	
	private:
		Mat *inputIMG;
		Mat inputORIG;
		Mat inputCOPY;
		Mat image_roi;
		Sample *sample;

		Rect roi;

		hsvStruct hsv;
		Thresh thresh;
		ParamsHSV params;

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		Mat canny_output;

		Point2f largestCenter;
		float largestRadius;
		vector<float> largestContour;
		int largest_contour_index;
		double largest_area;

		int tenPercent;

		bool firstThreshold = false;

		void noiseReduction();
		void thresholdHSV();
		void convertRGB2HSV();
		void findAllContours();
		void findLargestContour();

};

#endif	/* MATCHHSV_H */

