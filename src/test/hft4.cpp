// g++ -std=c++11 -o hft4 hft4.cpp `pkg-config opencv --cflags --libs`

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

#define PYTHAGOR(p1,p2) (sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y)))

int maxArea = 10;
int diversity = 500;

Mat canny_output;
int minThresh = 100;
int maxThresh = 255;

RNG rng(12345);

Mat gray;
vector<vector<Point> > msers;
double bestPoint = 10.0;
Rect bound;

struct Features {
  int numberOfHoles;
  double convexHullAreaRate;
  double minRectAreaRate;
  double skeletLengthRate;
  double contourAreaRate;
  bool full = false;
};

struct Colors {
    const Scalar RED = Scalar(0, 0, 255);
    const Scalar GREEN = Scalar(0, 255, 0);
    const Scalar BLUE = Scalar(255, 0, 0);
    const Scalar BLACK = Scalar(0, 0, 0);
    const Scalar WHITE = Scalar(255, 255, 255);
    const Scalar YELLOW = Scalar(0, 255, 255);
    const Scalar LIGHT_GRAY = Scalar(100, 100, 100);
};

Colors colors;

Mat mserToMat(vector<Point> *mser)
{
    int minX = min_element(mser->begin(), mser->end(), [] (Point &p1, Point &p2) { return p1.x < p2.x; })[0].x;
    int minY = min_element(mser->begin(), mser->end(), [] (Point &p1, Point &p2) { return p1.y < p2.y; })[0].y;
    int maxX = max_element(mser->begin(), mser->end(), [] (Point &p1, Point &p2) { return p1.x < p2.x; })[0].x;
    int maxY = max_element(mser->begin(), mser->end(), [] (Point &p1, Point &p2) { return p1.y < p2.y; })[0].y;

    Mat color(maxY - minY, maxX - minX, CV_8UC3);
    
    for_each(mser->begin(), mser->end(), [&] (Point &p) 
    {
        Point newPoint = Point(p.x - minX, p.y - minY);
        line(color, newPoint, newPoint, colors.WHITE);
    });    
    Mat gray;
    cvtColor(color, gray, CV_BGRA2GRAY);
    
    return gray;
}

int findHoles(Mat *img){
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    Canny(*img, canny_output, minThresh, minThresh*2, 3 );
    findContours(*img, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
    
    int result = 0;
    for (size_t i = 0; i < hierarchy.size(); i++) { 
        if (hierarchy[i][3] != - 1) result++;
    }
    
    return result;
}

int skeletLength(Mat *mserImg)
{
    Mat img;
    mserImg->copyTo(img);
    
    threshold(img, img, 127, 255, THRESH_BINARY); 
    Mat skel(img.size(), CV_8UC1, Scalar(0));
    Mat temp;
    Mat eroded;
    
    Mat element = getStructuringElement(MORPH_CROSS, Size(3, 3));
    bool done;      
    do
    {
        erode(img, eroded, element);
        dilate(eroded, temp, element); // temp = open(img)
        subtract(img, temp, temp);
        bitwise_or(skel, temp, skel);
        eroded.copyTo(img);
        
        done = (countNonZero(img) == 0);
    } while (!done);
    
    return countNonZero(skel);
}

double contourAreas(Mat *img)
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(*img, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    
    for (size_t i = 0; i < hierarchy.size(); i++) { 
        if (hierarchy[i][3] == -1) {
            double area = contourArea(contours[i]);
            if (area > 0) return area;
        }
    }
    
    return 0.0;
}

Features extractFeature(vector<Point> *mser){

    Features result;

    Mat mserImg = mserToMat(mser);

    erode(mserImg, mserImg, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
    dilate(mserImg, mserImg, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
    dilate(mserImg, mserImg, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
    erode(mserImg, mserImg, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));

    Mat drawingT = Mat::zeros( mserImg.cols+20,mserImg.rows+20, CV_8UC3 );
    // bitwise_not(drawingT,drawingT);
    cvtColor(drawingT, drawingT, CV_BGR2GRAY);

    cv::Rect roiT( cv::Point( 10, 10 ), mserImg.size() );
    mserImg.copyTo( drawingT( roiT ) );

    imshow("DRAAAAAAAWING", drawingT);
    waitKey(0);

    if (drawingT.cols <= 2 || drawingT.rows <= 2){
        cout << "null" << endl; 
    }

    RotatedRect minRect = minAreaRect(*mser);
    int numberOfHoles = findHoles(&drawingT);
    vector<Point> convexHull;
    cv::convexHull(*mser, convexHull);
    double convexHullAreaRate = (double)mser->size() / contourArea( convexHull );
    double minRectAreaRate = (double)mser->size() / (double) minRect.size.area();
    int leng = skeletLength(&drawingT);
    double skeletLengthRate = (double)leng / (double) mser->size();
    double contourArea = contourAreas(&drawingT);

    if (contourArea == 0.0){
        return result;
    }
    double contourAreaRate = (double)mser->size() / contourArea;
    if (contourAreaRate > 1.0){
        return result;
    }

    cout << "skeletLengthRate: " << skeletLengthRate << endl;
    cout << "minRectAreaRate: " << minRectAreaRate << endl;
    cout << "convexHullAreaRate: " << convexHullAreaRate << endl;
    cout << "numberOfHoles: " << numberOfHoles << endl;
    cout << "contourAreaRate: " << contourAreaRate << endl;

    result.numberOfHoles = (double)numberOfHoles;
    result.convexHullAreaRate = convexHullAreaRate;
    result.minRectAreaRate = minRectAreaRate;
    result.skeletLengthRate = skeletLengthRate;
    result.contourAreaRate = contourAreaRate;
    result.full = true;

    return result;
}

bool matchTemplate(Features featuresTPL, Features featuresMSER)
{
    if (featuresTPL.numberOfHoles != featuresMSER.numberOfHoles) { 
        return false; 
    }    
    if ((featuresTPL.convexHullAreaRate - featuresMSER.convexHullAreaRate) > 0.05) {
        return false;
    }
    if ((featuresTPL.minRectAreaRate - featuresMSER.minRectAreaRate) > 0.05) {
        return false;
    }
    if ((featuresTPL.skeletLengthRate - featuresMSER.skeletLengthRate) > 0.02) {
        return false;
    }
    if ((featuresTPL.contourAreaRate - featuresMSER.contourAreaRate) > 0.1) {
        return false;
    }
    
    return true;
}

vector<Point> maxMser(Mat *gray)
{

  double newMaxArea = maxArea/(double)1000;
  double newDiversity = diversity/(double)1000;

  MSER mser(21, (int)(newMaxArea*gray->cols*gray->rows), (int)(newDiversity*gray->cols*gray->rows), 1, 0.7);

  vector<vector<Point> > msers;

  mser(*gray, msers);

  if (msers.size() == 0) return vector<Point>();
  
  vector<Point> mserF = max_element(msers.begin(), msers.end(), [] (vector<Point> &m1, vector<Point> &m2) {
      return m1.size() < m2.size();
  })[0];
  
  return mserF;
}

Mat getPerspectiveMatrix(Point2f points[], Size2f size)
{
    Point2f dst[] = { 
        Point2f(0, 0), 
        Point2f(size.width, 0), 
        Point2f(size.width, size.width), 
        Point2f(0, size.width) 
    };
    bool first = PYTHAGOR(points[0], points[1]) < PYTHAGOR(points[1], points[2]);
    Point2f src[] = {
        first ? points[0] : points[1],
        first ? points[1] : points[2],
        first ? points[2] : points[3],
        first ? points[3] : points[0]
    };
    
    Mat result = getPerspectiveTransform(src, dst);
    return result;
}

Mat normalizeImage(Mat *image, Mat *M ,float size)
{
    Mat dst(Size(size, size), image->type());
    warpPerspective(*image, dst, *M, dst.size(), INTER_LINEAR, BORDER_DEFAULT, Scalar(1, 1, 1));

    return dst;
}

int main( int argc, char** argv ){
  
  Mat inputTPl, imageTPL, imageTPL_GRAY;
    Mat lambda( 2, 4, CV_32FC1 );
    Mat drawing = Mat::zeros( 200,200, imageTPL_GRAY.type() );
    lambda = Mat::zeros( 200, 200, imageTPL_GRAY.type() );
    Point2f inputQuad[4];
    Point2f outputQuad[4];

    Rect boundRect;
    vector<vector<Point> > contours;
    outputQuad[0] = Point2f( 0,0 );
    outputQuad[1] = Point2f( 200,0);
    outputQuad[2] = Point2f( 200,200);
    outputQuad[3] = Point2f( 0,200  );

    inputTPl = imread( argv[1]);
    cvtColor(inputTPl, imageTPL_GRAY, COLOR_BGR2GRAY);

    vector<Point> maxMserTPL;
    RotatedRect rect;
    Point2f points[4];
    Mat M; Mat normalizedImage;
    Mat mserImg;
    Mat drawing1;

    namedWindow("TEMPLATE CALIBRATION", WINDOW_NORMAL);
    createTrackbar("Max Area", "TEMPLATE CALIBRATION", &maxArea, 100);
    createTrackbar("Diversity", "TEMPLATE CALIBRATION", &diversity, 1000);

    while(true){

        maxMserTPL = maxMser(&imageTPL_GRAY);

        mserImg = mserToMat(&maxMserTPL);

        rect = minAreaRect(maxMserTPL);
        rect.points(points);

        M = getPerspectiveMatrix(points,rect.size);
        normalizedImage = normalizeImage(&imageTPL_GRAY,&M,rect.size.width);

        drawing1 = Mat::zeros( normalizedImage.cols+20,normalizedImage.rows+20, CV_8UC3 );
        bitwise_not(drawing1,drawing1);
        cvtColor(drawing1, drawing1, CV_BGR2GRAY);

        cout << " SIZE IS======== " << drawing1.size() << " = " << drawing1.cols/2 << " x " << drawing1.rows/2 << endl;
        cout << " SIZE IS======== " << normalizedImage.size() << " = " << normalizedImage.cols/2 << " x " << normalizedImage.rows/2 << endl;

        cv::Rect roi( cv::Point( 10, 10 ), normalizedImage.size() );
        normalizedImage.copyTo( drawing1( roi ) );      

        imshow("TEMPLATE CALIBRATION",drawing1);

        if(waitKey(30) >= 0){
            
            break;
        }
    }

    cvDestroyWindow("TEMPLATE CALIBRATION");
    waitKey(1);
    waitKey(1);
    waitKey(1);
    waitKey(1);
    waitKey(1);
    waitKey(1);
    waitKey(1);
    waitKey(1);

    vector<Point> normalizedMser = maxMser(&drawing1);

    Features featuresTPL = extractFeature(&normalizedMser);

    cout << "PRINTING FROM MAIN: " << featuresTPL.numberOfHoles << "\t"
                            << featuresTPL.convexHullAreaRate << "\t"
                            << featuresTPL.minRectAreaRate << "\t"
                            << featuresTPL.skeletLengthRate << "\t"
                            << featuresTPL.contourAreaRate << endl;

  return 0;

}
