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
Mat sourceORIG2;
Mat sourceGRAY;
Mat sourceGRAY2;
Mat sourceGRAY_BLUR;
Mat sourceGRAY_BLUR2;
Mat cannyOut;
Mat drawing;

Rect boundRect;
double maxArea = 0.0;

Point point1, point2;
int drag = 0;
Rect rect;
Mat img, roiImg;
int select_flag = 0;
bool go_fast = false;
int k;

Mat mytemplate;
Mat src2;

Mat TplMatch(Mat &img, Mat &mytemplate);
Point minmax( Mat &result );
void track();
void mouseHandler(int, int, int, int, void *);

int main(int argc, char** argv)
{

    sourceORIG = imread( argv[1] );
    sourceORIG2 = imread( argv[2] );
    cvtColor( sourceORIG, sourceGRAY, CV_BGR2GRAY );
    cvtColor( sourceORIG2, sourceGRAY2, CV_BGR2GRAY );

    GaussianBlur( sourceGRAY, sourceGRAY_BLUR, Size(3,3), 3.0 );

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
    mytemplate = drawing;

    GaussianBlur( sourceGRAY2, sourceGRAY_BLUR2, Size(7,7), 3.0 );
    imshow( "image", sourceGRAY_BLUR2 );

    track();

    imshow("image", sourceGRAY2);

    imshow(windowName,drawing);
    waitKey(0);

    return 0;
}

Mat TplMatch( Mat &sourceGRAY_BLUR2, Mat &mytemplate )
{
  Mat result;

  imshow("sourceGRAY2",sourceGRAY_BLUR2);
  imshow("mytemplate",mytemplate);
  cvtColor( mytemplate, mytemplate, CV_BGR2GRAY );
  cout << "SOURCEGRAYBLUR2: " << sourceGRAY_BLUR2.type() << endl;
  cout << "mytemplate: " << mytemplate.type() << endl;
  waitKey(0);
  


  matchTemplate( sourceGRAY_BLUR2, mytemplate, result, CV_TM_SQDIFF_NORMED );
  normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

  return result;
}

Point minmax( Mat &result )
{
  double minVal, maxVal;
  Point  minLoc, maxLoc, matchLoc;

  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
  matchLoc = minLoc;

  return matchLoc;
}

void track()
{

    Mat result  =  TplMatch( sourceGRAY_BLUR2, mytemplate );
    Point match =  minmax( result ); 

    rectangle( sourceGRAY_BLUR2, match, Point( match.x + mytemplate.cols , match.y + mytemplate.rows ), CV_RGB(255, 255, 255), 0.5 );

    std::cout << "match: " << match << endl;

    Rect ROI = cv::Rect( match.x, match.y, mytemplate.cols, mytemplate.rows );
    roiImg = sourceGRAY_BLUR2( ROI );
    roiImg.copyTo(mytemplate);
    imshow( "roiImg", roiImg );
    waitKey(0);
}