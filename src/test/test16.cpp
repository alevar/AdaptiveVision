#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    VideoCapture source(0); // open the default camera
    if(!source.isOpened())  // check if we succeeded
    {
        return -1;
    }

    Mat box;
    Mat imageToSend;
    source >> box;

    // Mat box = imread("logoT.jpg",1);
    Mat gray;
    cvtColor(box, gray, CV_BGR2GRAY);
    // bitwise_not ( gray, gray );
    
    //MSER ms(5,60, 14400, .25, .4, 100, 1.01, 0.003, 5);
    MSER ms;
    vector<vector<Point>> regions;
    ms(gray, regions);
    cout << "SIZE==== "<< regions.size() << endl;
    for (int i = 0; i < regions.size(); i++)
    {
        ellipse(gray, fitEllipse(regions[i]), Scalar(255));
    }
    imshow("mser", gray);
    waitKey(0);
    return 0;
}