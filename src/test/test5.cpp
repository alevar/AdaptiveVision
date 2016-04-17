#include <iostream>
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <sstream>
#include <vector>

using namespace cv;
using namespace std;

int ratio = 3;
int kernel_size = 3;

int lowThreshold = 100;
int const max_lowThreshold = 100;
string windowName = "Image";

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

Mat findObject();
void displayFinal(int,void*);

int main(int argc, char** argv)
{

    sourceORIG = imread( argv[1] );
    namedWindow( windowName, CV_WINDOW_AUTOSIZE );
    createTrackbar( "Min Threshold:", windowName, &lowThreshold, max_lowThreshold, displayFinal );
    displayFinal(0,0);

    return 0;
}

Mat findObject(){
    cvtColor( sourceORIG, sourceGRAY, CV_BGR2GRAY );

    imshow("gray",sourceGRAY);

    blur( sourceGRAY, sourceGRAY_BLUR, Size(3,3) );

    Canny( sourceGRAY_BLUR, cannyOut, lowThreshold, lowThreshold*ratio, kernel_size );
    cv::dilate(cannyOut, cannyOut, cv::Mat(), cv::Point(-1,-1));

    findContours( cannyOut, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    drawing = Mat::zeros( cannyOut.size(), CV_8UC3 );

    vector<Point> approxShape;
    for(size_t i = 0; i < contours.size(); i++){
        approxPolyDP(contours[i], approxShape, 5, true);
        drawContours(drawing, contours, i, Scalar(255, 255, 255), CV_FILLED);
    }
    bitwise_not ( drawing, drawing );

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
    return drawing;
}

void displayFinal(int,void*){

    drawing = findObject();

    imshow(windowName,drawing);

    waitKey(0);
}