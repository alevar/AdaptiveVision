#include "declarations.h"

int main()
{
	VideoCapture cap(0);
	Mat src1, src2, src3;
	while(true)
	{
		cap >> src1;
		src2 = src1.clone();
		src3 = src1.clone();
		sift(src1, "SIFT", "sift.jpg");
		mser(src2, "MSER", "mser.jpg");
		surf(src3, "SURF", "surf.jpg");
		cvWaitKey(20);
	}
	/*
	src1 = imread("popcorn/popcorn2.jpg");
	src2 = src1.clone();
	src3 = src1.clone();
	sift(src1, "SIFT translation", "sift_translation.jpg");
	mser(src2, "MSER translation", "mser_translation.jpg");
	surf(src3, "SURF translation", "surf_translation.jpg");
	src1 = imread("popcorn/popcorn3.jpg");
	src2 = src1.clone();
	src3 = src1.clone();
	sift(src1, "SIFT rotation", "sift_rotation.jpg");
	mser(src2, "MSER rotation", "mser_rotation.jpg");
	surf(src3, "SURF rotation", "surf_rotation.jpg");
	src1 = imread("popcorn/popcorn4.jpg");
	src2 = src1.clone();
	src3 = src1.clone();
	sift(src1, "SIFT zoom", "sift_zoom.jpg");
	mser(src2, "MSER zoom", "mser_zoom.jpg");
	surf(src3, "SURF zoom", "surf_zoom.jpg");
	*/
	waitKey(0);
	return(0);
}