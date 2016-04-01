/*=====================================
============= CLASS SAMPLE ============
=======================================
==== Clas for creating images that ====
= are sent to the server for analysis =
=====================================*/

#ifndef CANVAS_H
#define	CANVAS_H

#include <string>
#include <vector>
#include <stdarg.h>

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

class Canvas {

	public:
		
		Canvas();
		Canvas(vector<Mat>);
		void build(vector<Mat>);
		void show();
		Mat getMat();
		virtual ~Canvas();
	
	private:

		Mat img;

	    int size;
	    int i;
	    int m, n;
	    int x, y;
	 
	    int width, height;

	    float scale;
	    int max;

	    Mat resultCanvas;

};

#endif	/* CANVAS_H */

