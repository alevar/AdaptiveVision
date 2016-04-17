#include "declarations.h"

void mser(Mat img, char * window_name, char *filename)
{
	cout << filename << endl;
	MSER mser(5,60, 14400, .25, .4, 100, 1.01, 0.003, 5);
	vector<KeyPoint> keypoints;
	vector<vector<Point>> points;
	Mat gray;
	DWORD t1, t2, t;
	cvtColor(img, gray, CV_BGR2GRAY);
	t1 = ticks;
	mser(gray, points);
	t2 = ticks;
	t = t2 - t1;
	cout << "MSER features detected: " << (int)points.size() << endl << "Time to execute: " << t << endl;
	for(int i = 0; i < (int)points.size(); i++)
	{
		for(int j = 0; j < (int)points.at(i).size(); j++)
		{
			Point p = points.at(i).at(j);
			img.at<Vec3b>(p) = Vec3b(0,0,255);
		}
	}
	//img = drawCircles(img, keypoints);
	namedWindow(window_name);
	imshow(window_name, img);
	imwrite(filename, img);
}