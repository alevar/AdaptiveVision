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
#include <stdio.h>
#include <string>

using namespace std;
using namespace cv;

/// Global Variables
Mat img, templ, result;
Mat imgGRAY, templGRAY;
Mat dstIMG, dstTMPL;
Mat detected_edges;
Mat detected_edges2;
string image_window = "Source Image";
string result_window = "Result window";

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;

int match_method;
int max_Trackbar = 5;

/// Function Headers
void MatchingMethod( int, void* );

/** @function main */
int main( int argc, char** argv )
{
  /// Load image and template
  img = imread( argv[1], 1 );
  templ = imread( argv[2], 1 );

  dstIMG.create( dstIMG.size(), dstIMG.type() );
  dstTMPL.create( dstTMPL.size(), dstTMPL.type() );

  cvtColor( img, imgGRAY, CV_BGR2GRAY );
  cvtColor( templ, templGRAY, CV_BGR2GRAY );

  namedWindow( image_window, CV_WINDOW_AUTOSIZE );
  namedWindow( result_window, CV_WINDOW_AUTOSIZE );
  string trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
  createTrackbar( trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod );

  MatchingMethod( 0, 0 );

  waitKey(0);
  return 0;
}


void MatchingMethod( int, void* )
{

  blur( imgGRAY, detected_edges, Size(3,3) );
  blur( templGRAY, detected_edges2, Size(3,3) );
  Canny( detected_edges, detected_edges, 100, 100*ratio, kernel_size );
  Canny( detected_edges2, detected_edges2, 100, 100*ratio, kernel_size );
  bitwise_not ( detected_edges, detected_edges );
  bitwise_not ( detected_edges2, detected_edges2 );

  /// Source image to display
  Mat img_display;
  detected_edges.copyTo( img_display );

  /// Create the result matrix
  int result_cols =  detected_edges.cols - detected_edges2.cols + 1;
  int result_rows = detected_edges.rows - detected_edges2.rows + 1;

  result.create( result_rows, result_cols, CV_32FC1 );

  /// Do the Matching and Normalize
  matchTemplate( detected_edges, detected_edges2, result, match_method );
  normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

  /// Localizing the best match with minMaxLoc
  double minVal; double maxVal; Point minLoc; Point maxLoc;
  Point matchLoc;

  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

  /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
  if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
    { matchLoc = minLoc; }
  else
    { matchLoc = maxLoc; }

  /// Show me what you got
  rectangle( img_display, matchLoc, Point( matchLoc.x + detected_edges2.cols , matchLoc.y + detected_edges2.rows ), Scalar::all(0), 2, 8, 0 );
  rectangle( result, matchLoc, Point( matchLoc.x + detected_edges2.cols , matchLoc.y + detected_edges2.rows ), Scalar::all(0), 2, 8, 0 );

  imshow( image_window, img_display );
  imshow( result_window, result );

  return;
}