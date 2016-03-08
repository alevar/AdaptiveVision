/*==================================================
==================== TEMPLATE CLASS ================
====================================================
===== Class intended for creation of Templates =====
===== Templates are used against the incoming  =====
===== images, to identify similar objects	   =====
==================================================*/

#ifndef TEMPLATE_H
#define	TEMPLATE_H

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

#define PYTHAGOR(p1,p2) (sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y)))

class Template {

	public:

		Template();
		Template(Mat);		
		Mat getTemplate();
		virtual ~Template();

		Mat imageTPL;
		Mat lambda = Mat( 2, 4, CV_32FC1 );
	    Mat drawing = Mat::zeros( 200,200, imageTPL_GRAY.type() );
	    Point2f inputQuad[4];
	    Point2f outputQuad[4];
	    Rect boundRect;
	    vector<vector<Point> > contours;
	    
	
	private:

		Mat getPerspectiveMatrix(Point2f[], Size2f);
		Mat normalizeImage(Mat*, Mat* ,float);
		vector<Point> maxMser(Mat*);
		void detectRegions(Mat &, vector<vector<Point> > &);

		Mat inputTPL, imageTPL_GRAY, M;

	    Point2f points[4];
	    RotatedRect rect;
	    vector<Point> maxMserTPL;

		int delta = 5;
		int minArea = 100;
		int maxArea = 14400;
		double maxVariation = 0.25;
		double minDiversity = 0.2;
		int maxEvolution = 200;
		double areaThreshold = 1.01;
		double minMargin = 0.003;
		int edgeBlurSize = 0;

		const Scalar RED = Scalar(0, 0, 255);
		const Scalar GREEN = Scalar(0, 255, 0);
		const Scalar BLUE = Scalar(255, 0, 0);
		const Scalar BLACK = Scalar(0, 0, 0);
		const Scalar WHITE = Scalar(255, 255, 255);
		const Scalar YELLOW = Scalar(0, 255, 255);
		const Scalar LIGHT_GRAY = Scalar(100, 100, 100);

		MserFeatureDetector mserDetector = MserFeatureDetector(delta, minArea, maxArea,maxVariation, minDiversity, maxEvolution,areaThreshold, minMargin, edgeBlurSize);

    	vector<vector<Point> > msers;

};

#endif	/* TEMPLATE_H */

