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
#include <stdlib.h>
#include <stdio.h>

#include <string>

// http://cs.gmu.edu/~kosecka/cs682/cs682-template-matching.pdf
// http://stackoverflow.com/questions/9658531/how-to-identify-different-objects-in-an-image

using namespace cv;

/// Global variables

Mat src, src_gray;
Mat src2, src2_gray;
Mat dst, detected_edges;
Mat dst2, detected_edges2;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
string window_name = "Edge Map";
string window_name2 = "Edge Map2";

Mat img; Mat templ; Mat result;
char* image_window = "Source Image";
char* result_window = "Result window";

int match_method;
int max_Trackbar = 5;

void MatchingMethod(int,void*);

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void CannyThreshold(int, void*)
{
  /// Reduce noise with a kernel 3x3
  blur( src_gray, detected_edges, Size(3,3) );
  blur( src2_gray, detected_edges2, Size(3,3) );

  /// Canny detector
  Canny( detected_edges, detected_edges, 100, 100*ratio, kernel_size );
  Canny( detected_edges2, detected_edges2, lowThreshold, lowThreshold*ratio, kernel_size );

  /// Using Canny's output as a mask, we display our result
  dst = Scalar::all(0);
  dst2 = Scalar::all(0);

  src.copyTo( dst, detected_edges);
  src2.copyTo( dst2, detected_edges2);

  /// Create windows
  namedWindow( image_window, CV_WINDOW_AUTOSIZE );
  namedWindow( result_window, CV_WINDOW_AUTOSIZE );

  /// Create Trackbar
  char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
  createTrackbar( trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod );

  MatchingMethod( 0, 0 );

// KeyPoints

  // int minHessian = 400;

  // SurfFeatureDetector detector( minHessian );

  // std::vector<KeyPoint> keypoints_object, keypoints_scene;

  // detector.detect( detected_edges, keypoints_object );
  // detector.detect( detected_edges2, keypoints_scene );

  // //-- Step 2: Calculate descriptors (feature vectors)
  // SurfDescriptorExtractor extractor;

  // Mat descriptors_object, descriptors_scene;

  // extractor.compute( detected_edges, keypoints_object, descriptors_object );
  // extractor.compute( detected_edges2, keypoints_scene, descriptors_scene );

  // //-- Step 3: Matching descriptor vectors using FLANN matcher
  // FlannBasedMatcher matcher;
  // std::vector< DMatch > matches;
  // matcher.match( descriptors_object, descriptors_scene, matches );

  // double max_dist = 0; double min_dist = 100;

  // //-- Quick calculation of max and min distances between keypoints
  // for( int i = 0; i < descriptors_object.rows; i++ )
  // { double dist = matches[i].distance;
  //   if( dist < min_dist ) min_dist = dist;
  //   if( dist > max_dist ) max_dist = dist;
  // }

  // printf("-- Max dist : %f \n", max_dist );
  // printf("-- Min dist : %f \n", min_dist );

  // //-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
  // std::vector< DMatch > good_matches;

  // for( int i = 0; i < descriptors_object.rows; i++ )
  // { if( matches[i].distance < 3*min_dist )
  //    { good_matches.push_back( matches[i]); }
  // }

  // Mat img_matches;
  // drawMatches( detected_edges, keypoints_object, detected_edges2, keypoints_scene,
  //              good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
  //              vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

  // //-- Localize the object
  // std::vector<Point2f> obj;
  // std::vector<Point2f> scene;

  // for( int i = 0; i < good_matches.size(); i++ )
  // {
  //   //-- Get the keypoints from the good matches
  //   obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
  //   scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
  // }

  // Mat H = findHomography( obj, scene, CV_RANSAC );

  // //-- Get the corners from the image_1 ( the object to be "detected" )
  // std::vector<Point2f> obj_corners(4);
  // obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( detected_edges.cols, 0 );
  // obj_corners[2] = cvPoint( detected_edges.cols, detected_edges.rows ); obj_corners[3] = cvPoint( 0, detected_edges.rows );
  // std::vector<Point2f> scene_corners(4);

  // perspectiveTransform( obj_corners, scene_corners, H);

  // //-- Draw lines between the corners (the mapped object in the scene - image_2 )
  // line( img_matches, scene_corners[0] + Point2f( detected_edges.cols, 0), scene_corners[1] + Point2f( detected_edges.cols, 0), Scalar(0, 255, 0), 4 );
  // line( img_matches, scene_corners[1] + Point2f( detected_edges.cols, 0), scene_corners[2] + Point2f( detected_edges.cols, 0), Scalar( 0, 255, 0), 4 );
  // line( img_matches, scene_corners[2] + Point2f( detected_edges.cols, 0), scene_corners[3] + Point2f( detected_edges.cols, 0), Scalar( 0, 255, 0), 4 );
  // line( img_matches, scene_corners[3] + Point2f( detected_edges.cols, 0), scene_corners[0] + Point2f( detected_edges.cols, 0), Scalar( 0, 255, 0), 4 );

  //-- Show detected matches
  imshow( "Good Matches & Object detection", img_matches );

}

void MatchingMethod( int, void* )
{
  std::cout<<"HEEEEEEEEEEEEE"<<std::endl;
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

/** @function main */
int main( int argc, char** argv )
{
  src = imread( argv[1] );
  src2 = imread( argv[2] );

  /// Create a matrix of the same type and size as src (for dst)
  dst.create( src.size(), src.type() );
  dst2.create( src2.size(), src2.type() );

  /// Convert the image to grayscale
  cvtColor( src, src_gray, CV_BGR2GRAY );
  cvtColor( src2, src2_gray, CV_BGR2GRAY );

  /// Create a window
  namedWindow( window_name, CV_WINDOW_AUTOSIZE );
  namedWindow( window_name2, CV_WINDOW_AUTOSIZE );

  /// Create a Trackbar for user to enter threshold
  createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
  createTrackbar( "Min Threshold:", window_name2, &lowThreshold, max_lowThreshold, CannyThreshold );

  /// Show the image
  CannyThreshold(0, 0);

  /// Wait until user exit program by pressing a key
  waitKey(0);

  return 0;
}