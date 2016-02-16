/*==================================================
=========== Object Identification CLASS ============
====================================================
=== Object Identification using machine learning tools
==================================================*/

// http://aishack.in/tutorials/sift-scale-invariant-feature-transform-scale-space/
// http://aishack.in/tutorials/canny-edge-detector/

// Procedure
/*=============================================================
1. If an argument "identify" is supplied to the program at startup
2. Start a countdown (5-1) & start capturing the image
3. After the first countdown start a new countdown (5-1)
4. Capture the new data: this time the new shape will be presented
5. Start the new countdown (5-1) - signal for shape removal
6. By image subtraction find the new shape to track
=============================================================*/

// Additional changes to the previous implementation
/*=============================================================
1. If an argument is given to the client
2. When the connection between the client and the server is initiated
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

