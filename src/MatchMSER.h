/*==================================================
==================== TEMPLATE CLASS ================
====================================================
===== Class intended for creation of Templates =====
===== Templates are used against the incoming  =====
===== images, to identify similar objects	   =====
==================================================*/

#ifndef MATCHMSER_H
#define	MATCHMSER_H

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

#include "Template.h"

using namespace std;
using namespace cv;

#define PYTHAGOR(p1,p2) (sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y)))

struct Features {
	int numberOfHoles;
	double convexHullAreaRate;
	double minRectAreaRate;
	double skeletLengthRate;
	double contourAreaRate;
	bool full = false;
};

struct Colors {
    const Scalar RED = Scalar(0, 0, 255);
    const Scalar GREEN = Scalar(0, 255, 0);
    const Scalar BLUE = Scalar(255, 0, 0);
    const Scalar BLACK = Scalar(0, 0, 0);
    const Scalar WHITE = Scalar(255, 255, 255);
    const Scalar YELLOW = Scalar(0, 255, 255);
    const Scalar LIGHT_GRAY = Scalar(100, 100, 100);
};

// struct Params {
// 	int delta 				= 5;
// 	int minArea 			= 100;
// 	int maxArea 			= 14400;
// 	double maxVariation 	= 0.25;
// 	double minDiversity 	= 0.2;
// 	int maxEvolution 		= 200;
// 	double areaThreshold 	= 1.01;
// 	double minMargin 		= 0.003;
// 	int edgeBlurSize 		= 0;
// };

class MatchMSER {

	public:

		MatchMSER();
		MatchMSER(Mat*);
		MatchMSER(Mat*, Mat*);
		void setTemplate(Mat*);
		void setTemplate(vector<Point>);
		void setImage(Mat*);
		void set(Mat*,Mat*);
		void setParams(int, int);
		Mat findMatch(Mat);
		Mat findMatchTPL(Mat);
		vector<int> getHSV();

		bool publicMatch = false;

		// void setParams(int*,int*,int*,double*,double*,int*,double*,double*,int*);
		virtual ~MatchMSER();	    
	
	private:
		Mat *imgTPL;
		Mat *imgMAT;

		void detectRegions(Mat &, vector<vector<Point> > &);
		Mat mserToMat(vector<Point>*);
		int findHoles(Mat*);
		int skeletLength(Mat*);
		double contourAreas(Mat*);
		Features extractFeature(vector<Point>*);
		Features extractFeatureTPL(vector<Point>*);
		bool matchTemplate(Features, Features);
		double distace(Features*, Features*);
		Mat processImage(Mat);
		vector<Point> maxMser(Mat*);
		Mat getPerspectiveMatrix(Point2f[], Size2f);
		Mat normalizeImage(Mat*, Mat*,float);

		Features features;
		Colors colors;
		// Params params;

		vector<Point> normalizedMser;
		Features featuresTPL;

		Mat canny_output;
		int minThresh = 100;
		int maxThresh = 255;

		Mat bestMSER;
		Mat clone;
		Mat mask;
		int bestHUE;
		int bestSAT;
		int bestVAL;

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		int largest_area = 0;
		int largest_contour_index=0;

		int maxArea = 10;
		int diversity = 500;

		vector<int> updatedHSV;
		bool foundMatch;

		// MserFeatureDetector mserDetector = MserFeatureDetector(params.delta, params.minArea, params.maxArea,
		// 										params.maxVariation, params.minDiversity, params.maxEvolution,
		// 										params.areaThreshold, params.minMargin, params.edgeBlurSize);

};

#endif	/* MATCHMSER_H */

