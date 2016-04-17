#include <iostream>
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

int minHessian = 50;
vector<cv::KeyPoint> keypointsTMPL, keypointsSCENE;
SurfDescriptorExtractor extractor;
SurfFeatureDetector detector( minHessian );
Mat descriptorsTMPL, descriptorsSCENE;
vector<cv::Point2f> obj_corners(4);
FlannBasedMatcher matcher;
vector<cv::vector<cv::DMatch > > matches;
vector<cv::DMatch > good_matches;
vector<cv::Point2f> obj;
vector<cv::Point2f> scene;
vector<cv::Point2f> scene_corners(4);
Mat H;
Mat img_matches;

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
    GaussianBlur( sourceGRAY2, sourceGRAY_BLUR2, Size(7,7), 3.0 );

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

    // http://stackoverflow.com/questions/24539273/how-to-find-out-how-good-a-match-is-in-opencv

    // GaussianBlur( mytemplate, mytemplate, Size(7,7), 3.0 );

    detector.detect( mytemplate, keypointsTMPL );
    detector.detect( sourceGRAY_BLUR2, keypointsSCENE );

    extractor.compute( mytemplate, keypointsTMPL, descriptorsTMPL );
    extractor.compute( sourceGRAY_BLUR2, keypointsSCENE, descriptorsSCENE );

    obj_corners[0] = (cvPoint(0,0));
    obj_corners[1] = (cvPoint(mytemplate.cols,0));
    obj_corners[2] = (cvPoint(mytemplate.cols,mytemplate.rows));
    obj_corners[3] = (cvPoint(0, mytemplate.rows));

    if(descriptorsSCENE.type()!=CV_32F) {
        descriptorsSCENE.convertTo(descriptorsSCENE, CV_32F);
    }
    if(descriptorsTMPL.type()!=CV_32F) {
        descriptorsTMPL.convertTo(descriptorsTMPL, CV_32F);
    }

    // if ( descriptorsTMPL.empty() )
    //    cvError(0,"MatchFinder","1st descriptor empty",__FILE__,__LINE__);    
    // if ( descriptorsSCENE.empty() )
    //    cvError(0,"MatchFinder","2nd descriptor empty",__FILE__,__LINE__);

    matcher.knnMatch( descriptorsTMPL, descriptorsSCENE, matches, 2);

    for(int i = 0; i < cv::min(sourceGRAY_BLUR2.rows-1,(int) matches.size()); i++)  {
        if((matches[i][0].distance < 0.6*(matches[i][1].distance)) && ((int) matches[i].size()<=2 && (int) matches[i].size()>0))  {
            good_matches.push_back(matches[i][0]);
            cout << "GOOD MATCHES" << endl;
        }
        cout << "MATCHES: " << matches[i].size() << endl;
    }
    // for( int i = 0; i < descriptorsTMPL.rows; i++ )
    // { 
    //   if( matches[i][0].distance < 100 )
    //   { 
    //       good_matches.push_back( matches[i][0]); 
    //   }
    // }

    drawMatches( sourceGRAY_BLUR2, keypointsSCENE, mytemplate, keypointsTMPL, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
               vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    if (good_matches.size() >= 4)  {

         for( int i = 0; i < good_matches.size(); i++ ) {
             //Get the keypoints from the good matches
             obj.push_back( keypointsTMPL[ good_matches[i].queryIdx ].pt );
             scene.push_back( keypointsSCENE[ good_matches[i].trainIdx ].pt );
         }

         H = findHomography( obj, scene, CV_RANSAC );

         perspectiveTransform( obj_corners, scene_corners, H);

         //Draw lines between the corners (the mapped object in the scene image )
         line( img_matches, scene_corners[0], scene_corners[1], cvScalar(0, 255, 0), 4 );

         line( img_matches, scene_corners[1], scene_corners[2], cvScalar( 0, 255, 0), 4 );

         line( img_matches, scene_corners[2], scene_corners[3], cvScalar( 0, 255, 0), 4 );

         line( img_matches, scene_corners[3], scene_corners[0], cvScalar( 0, 255, 0), 4 );
    }

    imshow( "Good Matches & Object detection", img_matches );

     // UIImage *resultimage = [UIImage imageWithCVMat:img_matches];
     // UIImageView *imageview = [[UIImageView alloc] initWithImage:resultimage];
     // imageview.frame = CGRectMake(0, 0, 320, 240);

     // UIImage *resultimage2 = [UIImage imageWithCVMat:mytemplate];
     // UIImageView *imageview2 = [[UIImageView alloc] initWithImage:resultimage2];
     // imageview2.frame = CGRectMake(0, 240, 320, 240);

    // GaussianBlur( sourceGRAY2, sourceGRAY_BLUR2, Size(7,7), 3.0 );
    // imshow( "image", sourceGRAY_BLUR2 );

    // track();

    // imshow("image", sourceGRAY2);

    // imshow(windowName,drawing);
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