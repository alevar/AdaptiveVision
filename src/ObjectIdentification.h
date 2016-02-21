/*==================================================
=========== Object Identification CLASS ============
====================================================
=== Object Identification using machine learning tools
==================================================*/

// http://aishack.in/tutorials/sift-scale-invariant-feature-transform-scale-space/
// http://aishack.in/tutorials/canny-edge-detector/
// http://www.jayrambhia.com/blog/sift-keypoint-matching-using-python-opencv/
// http://docs.opencv.org/2.4/doc/tutorials/features2d/feature_flann_matcher/feature_flann_matcher.html
// http://opencv-python-tutroals.readthedocs.org/en/latest/py_tutorials/py_feature2d/py_feature_homography/py_feature_homography.html

// Procedure
/*=============================================================
1. If an argument "identify" is supplied to the client program at startup
2. Start a countdown (5-1) & start capturing the image
3. After the first countdown start a new countdown (5-1)
4. Capture the new data: this time the new shape will be presented
	=> the shape should be of a specific color at the calibration
	=> the color would not matter afterwards during the execution
		-> during the execution of the program the shape will be matched using SIFT/FLANN keypoint matching
		-> the new Values will be calculated from the histogram based on the isolated object

5. Start the new countdown (5-1) - signal for shape removal
6. By image subtraction find the new shape to track
=============================================================*/

// Additional changes to the previous implementation
/*=============================================================
1. If an argument is given to the client
2. When the connection between the client and the server is initiated
	=> Communicate the fact that calibration needs to happen to the server right after the connection is established

3. Submit the final shape to the server
4. Server will be using that shape for object tracking
=============================================================*/

#ifndef OBJECTIDENTIFICATION_H
#define	OBJECTIDENTIFICATION_H

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

class ObjectIdentification {

	public:

		ObjectIdentification();
		ObjectIdentification(Mat);		
		virtual ~ObjectIdentification();
	
	private:

};

#endif	/* OBJECTIDENTIFICATION_H */

