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

using namespace std;
using namespace cv;

class MatchHSV {

	public:

		MatchHSV();
		virtual ~MatchHSV();	    
	
	private:

};

#endif	/* MATCHHSV_H */

