/*===============================================================
=================         COLOR TRACKING          ===============
=================================================================
===== The following program is intended for color detection =====
===== and data transmission between a Unix-based operating  =====
============	system and Arduino microprocessor.	=============
===== Please ensure that you have opencv package installed ======
=================== 	as well as LibSerial. 	=================
===============================================================*/

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

#include "omp.h" // PID LIBRARY

#include <iostream> // GENERAL PURPOSE LIBRARIES
#include <cmath> // GENERAL PURPOSE LIBRARIES

#include <string.h>
#include <unistd.h> 
#include <stdio.h>

using namespace cv; // USING THE NAMESPACE FOR OPENCV OBJECTS
using namespace std; // USING NAMESPACE FOR THE GENERAL PURPOSE OBJECTS

int size[2] = {640,480};

int main(int argc, char* argv[]){

	VideoCapture source1(1);
	VideoCapture source2(0);
	if (!source1.isOpened() && !source2.isOpened())  // if not success, exit program
	{
		cout << "Did you forget to switch the camera # ???" << endl;
		return -1;
	}

	double Width1 = source1.set(CV_CAP_PROP_FRAME_WIDTH,size[0]); //get the width of frames of the video
	double Height1 = source1.set(CV_CAP_PROP_FRAME_HEIGHT,size[1]); //get the height of frames of the video

	double Width2 = source2.set(CV_CAP_PROP_FRAME_WIDTH,size[0]); //get the width of frames of the video
	double Height2 = source2.set(CV_CAP_PROP_FRAME_HEIGHT,size[1]); //get the height of frames of the video
	
	Mat frame1;
	Mat frame2;

	Mat imageHSV1;
	Mat imageHSV2;

	source1 >> frame1;
	source2 >> frame2;

	while(true){

 		#pragma omp parallel sections private(frame1,source1,frame2,source2,imageHSV1,imageHSV2)
 		{
 			#pragma omp section
 			{
				
				
				source1.read(frame1);
				cout << "hello1" << endl;

				// namedWindow("Original",CV_WINDOW_AUTOSIZE);
				// imshow("Original", frame1);

				/* Setting up the Windows for projections */
				// namedWindow("2nd",CV_WINDOW_AUTOSIZE);
				// namedWindow("Original",CV_WINDOW_AUTOSIZE);
				// namedWindow("HSV",CV_WINDOW_AUTOSIZE);
				// namedWindow("Thresholding",CV_WINDOW_AUTOSIZE);
				// namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
				// namedWindow( "Contours2", CV_WINDOW_AUTOSIZE );
				// imshow( "Contours", drawing1 );		
				// imshow( "Contours2", drawing2 );		
				// imshow("Original", frame1);
				// imshow("2nd", frame2);
				// imshow("HSV", image1HSV);
				// imshow("Thresholding", image1THR);

			}
			
			#pragma omp section
			{
				source2.read(frame2);
				cout << "hello" << endl;


				// namedWindow("Original2",CV_WINDOW_AUTOSIZE);
				// imshow("Original2", frame2);

				/* Setting up the Windows for projections */
				// namedWindow("2nd",CV_WINDOW_AUTOSIZE);
				// namedWindow("Original",CV_WINDOW_AUTOSIZE);
				// namedWindow("HSV",CV_WINDOW_AUTOSIZE);
				// namedWindow("Thresholding",CV_WINDOW_AUTOSIZE);
				// namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
				// namedWindow( "Contours2", CV_WINDOW_AUTOSIZE );
				// imshow( "Contours", drawing1 );		
				// imshow( "Contours2", drawing2 );		
				// imshow("Original", frame1);
				// imshow("2nd", frame2);
				// imshow("HSV", image1HSV);
				// imshow("Thresholding", image1THR);

			}
		}	
		
	}

	return 0;

}

