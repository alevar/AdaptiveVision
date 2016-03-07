#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/nonfree/nonfree.hpp"
#include <iostream>
#include <sstream>
#include <vector>

using namespace cv;
using namespace std;

int ratio = 3;
int kernel_size = 3;

vector<vector<Point> > contours;
vector<vector<Point> > contours2;
vector<Vec4i> hierarchy;
vector<Vec4i> hierarchy2;

Mat sourceORIG;
Mat sourceGRAY;
Mat sourceGRAY_BLUR;
Mat cannyOut;
Mat drawing;

Rect boundRect;
double maxArea = 0.0;

Mat findObject(Mat source){
    cvtColor( source, sourceGRAY, CV_BGR2GRAY );

    imshow("gray",sourceGRAY);

    blur( sourceGRAY, sourceGRAY_BLUR, Size(3,3) );

    Canny( sourceGRAY_BLUR, cannyOut, 100, 100*ratio, kernel_size );
    cv::dilate(cannyOut, cannyOut, cv::Mat(), cv::Point(-1,-1));

    findContours( cannyOut, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    drawing = Mat::zeros( cannyOut.size(), CV_8UC3 );

    vector<Point> approxShape;
    for(size_t i = 0; i < contours.size(); i++){
        approxPolyDP(contours[i], approxShape, 5, true);
        drawContours(drawing, contours, i, Scalar(255, 0, 0), CV_FILLED);
    }

    vector<vector<Point> > contours_poly( contours.size() );
    for( int i = 0; i < contours.size(); i++ )
    { 
        double area = contourArea(contours[i]);
        if(area > maxArea) {
            maxArea = area;
            approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
            boundRect = boundingRect( Mat(contours_poly[i]) );
        }
    }

    drawing = drawing(boundRect);

    imshow("image",drawing);

    waitKey(0);
}

int main(int argc, char** argv)
{

    sourceORIG = imread( argv[1] );
    findObject(sourceORIG);

    return 0;
}