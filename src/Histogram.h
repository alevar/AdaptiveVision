/*==================================================
==================== Histogram CLASS =================
====================================================
==== Class is meant to hold the current Histogram ====
==== as well as all the information associated  ====
===================== with it. =====================
==================================================*/

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

using namespace std;
using namespace cv;

class Histogram {

	public:

		Histogram(Mat);
		Histogram();
		
		string calcHis();

		void toHistogram(Mat);
		
		virtual ~Histogram();

		string outHistogram;	             // Histogram as an array of bytes - contains opcode, type and length
	
	private:

};

#endif	/* HISTOGRAM_H */

