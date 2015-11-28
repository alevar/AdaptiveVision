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

#include "cameraDetect.h"

// g++ -std=c++11 -o  test test.cpp `pkg-config opencv --cflags --libs`

int main(){

	Mat original_frame;
	Mat original_frame2;

	VideoCapture source1(0);
	VideoCapture source2(1);

	float distance = 7.0;

	CameraDetect camera(&original_frame, &original_frame2, source1, source2);
	camera.setCameraDistance(distance);
	cout << "The distance between cameras is:	" << camera.getCameraDistance() << endl;

	camera.setHSV(30,59);
	vector<int> hsvValues = camera.getHSV();
	cout << "The HSV Values are:	" << endl;
	for(int i=0;i<hsvValues.size();i++){
		cout << hsvValues[i] << endl;
	}

	camera.setResolution(720,480);
	vector<int> resolution = camera.getResolution();
	cout << "The Resolution is:	" << resolution[0] << "x" << resolution[1] << endl;

	return 0;
}