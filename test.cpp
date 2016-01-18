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

#include <iostream>
#include <cmath>
#include <string.h>
#include <unistd.h> 
#include <stdio.h>

#include "classes/cameraDetect.h"

// g++ -std=c++11 -o  test test.cpp `pkg-config opencv --cflags --libs`

int main(){

	Mat *original_frame;

	CameraDetect camera(original_frame);

	return 0;
}