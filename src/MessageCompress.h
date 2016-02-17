/*==================================================
=========== Message Compression CLASS ==============
====================================================
=== Compresses Image Raw Data to allow faster transmission rate
==================================================*/

#ifndef MESSAGECOMPRESS_H
#define	MESSAGECOMPRESS_H

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

class MessageCompress {

	public:

		MessageCompress();
		MessageCompress(Mat);		
		virtual ~MessageCompress();
	
	private:

};

#endif	/* MESSAGECOMPRESS_H */

