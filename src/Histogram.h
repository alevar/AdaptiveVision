/*==================================================
==================== Histogram CLASS =================
====================================================
==== Class is meant to hold the current Histogram ====
==== as well as all the information associated  ====
===================== with it. =====================
==================================================*/

// http://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/histogram_calculation/histogram_calculation.html
// http://aishack.in/tutorials/drawing-histograms-opencv/
// http://codereview.stackexchange.com/questions/94434/histogram-of-a-rgb-image-using-opencv

#ifndef HISTOGRAM_H
#define	HISTOGRAM_H

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

#define MAX_BIT		256

using namespace std;
using namespace cv;

class Histogram {

	public:

		Histogram();
		Histogram(Mat);
		
		void calcHis();
		void calcHisHSV();
		void toHistogram(Mat);
		void showHist();
		
		virtual ~Histogram();

		Mat plot;

		// RGB

		int Red;
		int Green;
		int Blue;
		Vec3b intensityRGB;

		bool hisRGB;

		int HistR[MAX_BIT+1] = {0};
	    int HistG[MAX_BIT+1] = {0};
	    int HistB[MAX_BIT+1] = {0};

	    Mat HistPlotR = Mat (500, 256, CV_8UC3, Scalar(0, 0, 0));
	    Mat HistPlotG = Mat (500, 256, CV_8UC3, Scalar(0, 0, 0));
	    Mat HistPlotB = Mat (500, 256, CV_8UC3, Scalar(0, 0, 0));

	    // HSV

	    int Hue;
		int Val;
		int Sat;
		Vec3b intensityHSV;

	    bool hisHSV;

	    int HistH[MAX_BIT+1] = {0};
	    int HistS[MAX_BIT+1] = {0};
	    int HistV[MAX_BIT+1] = {0};

	    Mat HistPlotH = Mat (500, 256, CV_8UC3, Scalar(0, 0, 0));
	    Mat HistPlotS = Mat (500, 256, CV_8UC3, Scalar(0, 0, 0));
	    Mat HistPlotV = Mat (500, 256, CV_8UC3, Scalar(0, 0, 0));
	
	private:
		Mat imageHSV;
		Mat imageRGB;

		void convertRGB2HSV(Mat);

};

#endif	/* HISTOGRAM_H */

