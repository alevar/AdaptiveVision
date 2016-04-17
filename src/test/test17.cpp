
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/features2d/features2d.hpp" 
#include <iostream>
#include <stdio.h>      
  
using namespace std;  
using namespace cv;  
int main()  
{  
  
 // Mat inImg = imread("logoT1.jpg");

	VideoCapture source(0); // open the default camera
	if(!source.isOpened())  // check if we succeeded
	{
    	return -1;
	}

	Mat inImg;
	namedWindow("t");
	while(true){
		source >> inImg;  
	  
		Mat textImg;  
		cvtColor(inImg, textImg, CV_BGR2GRAY);
		// bitwise_not(inImg,inImg);
		//Extract MSER  
	   
		vector< vector< Point> > contours;  
		vector< Rect> bboxes;  
		MSER mser(21, (int)(0.01*textImg.cols*textImg.rows), (int)(0.5*textImg.cols*textImg.rows), 1, 0.7);
		// MSER mser(21, 10000, (int)(0.05*textImg.cols*textImg.rows), 1, 0.7);   
		mser(textImg, contours);   
	  
		for (int i = 0; i < (int)contours.size(); i++)  
		{  
	 		Rect bound = boundingRect(contours[i]);
			rectangle(inImg, bound, CV_RGB(0, 255, 0));  
		}  
	  
	  
		  
		imshow("t", inImg);
		if(waitKey(30) >= 0) break;  
		// waitKey(0);  
	}
  
}