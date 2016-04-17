// https://www.toptal.com/machine-learning/real-time-object-detection-using-mser-in-ios

// MAXIMALLY STABLE EXTREMAL REGIONS IMPLEMENTATION


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

//===========================================================
// PLAN FOR IMAGE SAMPLING
//===========================================================
// Client analyzes the image based on the HSV
// If client is able to detect an object Using the HSV then it crops the image to that object and sends it
// If the client is unable to detect an object - sends an entire frame uncropped
//===========================================================
// PLAN FOR INITIAL CALIBRATION
//===========================================================
// When the server is launched two pictures are displayed
// First is the template
// second is the object
// User adjusts the values for the object to be thresholded the best
// The thresholded image is used to determine the initial HSV values
//===========================================================

// REFER TO THE FOLLOWING FOR AN EXPLANATION WHY STRUCTS ARE FASTER AND BETTER
// http://eli.thegreenplace.net/2016/returning-multiple-values-from-functions-in-c/

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

int delta = 5;
int minArea = 100;
int maxArea = 14400;
double maxVariation = 0.25;
double minDiversity = 0.2;
int maxEvolution = 200;
double areaThreshold = 1.01;
double minMargin = 0.003;
int edgeBlurSize = 0;

const Scalar RED = Scalar(0, 0, 255);
const Scalar GREEN = Scalar(0, 255, 0);
const Scalar BLUE = Scalar(255, 0, 0);
const Scalar BLACK = Scalar(0, 0, 0);
const Scalar WHITE = Scalar(255, 255, 255);
const Scalar YELLOW = Scalar(0, 255, 255);
const Scalar LIGHT_GRAY = Scalar(100, 100, 100);

MserFeatureDetector mserDetector(delta, minArea, maxArea,maxVariation, minDiversity, maxEvolution,areaThreshold, minMargin, edgeBlurSize);

void detectRegions(Mat &gray, vector<vector<Point> > & vector){
    mserDetector(gray, vector);
}

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
        line(color, newPoint, newPoint, WHITE);
    });    
    Mat gray;
    cvtColor(color, gray, CV_BGRA2GRAY);
    
    return gray;
}

int findHoles(Mat *img){
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(*img, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
    
    if (hierarchy.size() == contours.size()) return 1;
    
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

    if (mserImg.cols <= 2 || mserImg.rows <= 2){
        cout << "null" << endl; 
    }

    RotatedRect minRect = minAreaRect(*mser);

    int numberOfHoles = findHoles(&mserImg);
    cout << "numberOfHoles: " << numberOfHoles << endl;

    vector<Point> convexHull;

    cv::convexHull(*mser, convexHull);
    double convexHullAreaRate = (double)mser->size() / contourArea( convexHull );
    cout << "convexHullAreaRate: " << convexHullAreaRate << endl;

    double minRectAreaRate = (double)mser->size() / (double) minRect.size.area();

    cout << "minRectAreaRate: " << minRectAreaRate << endl;

    int leng = skeletLength(&mserImg);
    double skeletLengthRate = (double)leng / (double) mser->size();

    cout << "skeletLengthRate: " << skeletLengthRate << endl;

    double contourArea = contourAreas(&mserImg);
    if (contourArea == 0.0){
        return result;
    }
    double contourAreaRate = (double)mser->size() / contourArea;
    if (contourAreaRate > 1.0){
        return result;
    }
    cout << "contourAreaRate: " << contourAreaRate << endl;

    // vector<double> result = {(double)numberOfHoles,convexHullAreaRate,minRectAreaRate,skeletLengthRate,contourAreaRate};

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

double distace(Features *featuresTPL, Features *featuresMSER)
{
    return 
    (featuresTPL->numberOfHoles == featuresMSER->numberOfHoles ? 1 : 10) * 
    (log(fabs(featuresTPL->convexHullAreaRate - featuresMSER->convexHullAreaRate)) + 
     log(fabs(featuresTPL->minRectAreaRate - featuresMSER->minRectAreaRate)) + 
     log(fabs(featuresTPL->skeletLengthRate - featuresMSER->skeletLengthRate)) + 
     log(fabs(featuresTPL->contourAreaRate - featuresMSER->contourAreaRate)));
}

// double distance(Features feature)
// {
//     return [self.logoTemplate distace: feature];
// }

Mat processImage(Mat &image, Features featuresTPL){
    Mat gray;
    cout << "START PROCESSIMAGE" << endl;
    cvtColor(image, gray, CV_BGRA2GRAY);
    cout << "STOP PROCESSIMAGE" << endl;
    vector<vector<Point> > msers;
    detectRegions(gray,msers);
    vector<Point> *bestMser = NULL;
    double bestPoint = 10.0;

    for_each(msers.begin(), msers.end(), [&] (vector<Point> &mser) 
    {
        Features featuresMSER = extractFeature(&mser);

        cout << "FEATURES: " << featuresMSER.numberOfHoles << "\t"
                            << featuresMSER.convexHullAreaRate << "\t"
                            << featuresMSER.minRectAreaRate << "\t"
                            << featuresMSER.skeletLengthRate << "\t"
                            << featuresMSER.contourAreaRate << endl;

        if(featuresMSER.full)            
        {

            cout << "ALL good" << endl;

            bool wellMatched = matchTemplate(featuresTPL,featuresMSER);

            if(wellMatched)
            {

                cout << "WELL MATCHED" << endl;
                double tmp = distance(&featuresTPL, &featuresMSER);
                cout << "TMP ================================= " << tmp << endl;
                if ( bestPoint > tmp ) {
                    bestPoint = tmp;
                    bestMser = &mser;
                }
            }
        }
    });

    if (bestMser)
    {
                
        Rect bound = boundingRect(*bestMser);
        rectangle(image, bound, GREEN, 3);
    }

    // if (msers.size() == 0) { 
    //     return image; 
    // };

    return image;
}

vector<Point> maxMser(Mat *gray)
{
    vector<vector<Point> > msers;

    detectRegions (*gray, msers);

    if (msers.size() == 0) return vector<Point>();
    
    vector<Point> mser = max_element(msers.begin(), msers.end(), [] (vector<Point> &m1, vector<Point> &m2) {
        return m1.size() < m2.size();
    })[0];
    
    return mser;
}

#define PYTHAGOR(p1,p2) (sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y)))

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

Mat drawCircles(Mat img, vector<KeyPoint> keypoints)
{
    for(int i = 0; i < (int)keypoints.size(); i++)
    {
        circle(img, keypoints.at(i).pt, keypoints.at(i).size, Scalar(0,0,255));
    }
    return img;
}

void mser(Mat img, char * window_name, char *filename)
{
    // cout << filename << endl;
    MSER mser(5,60, 14400, .25, .4, 100, 1.01, 0.003, 5);
    vector<KeyPoint> keypoints;
    vector<vector<Point>> points;
    Mat gray;

    cvtColor(img, gray, CV_BGR2GRAY);

    mser(gray, points);

    for(int i = 0; i < (int)points.size(); i++)
    {
        for(int j = 0; j < (int)points.at(i).size(); j++)
        {
            Point p = points.at(i).at(j);
            img.at<Vec3b>(p) = Vec3b(0,0,255);
        }
    }
    img = drawCircles(img, keypoints);
    namedWindow(window_name);
    imshow(window_name, img);
    // waitKey(0);
}

int main( int argc, char** argv )
{

//===================================================
// CREATING A TEMPLATE

    Mat inputTPl, imageTPL, imageTPL_GRAY;
    Mat lambda( 2, 4, CV_32FC1 );
    Mat drawing = Mat::zeros( 200,200, imageTPL_GRAY.type() );
    lambda = Mat::zeros( 200, 200, imageTPL_GRAY.type() );
    Point2f inputQuad[4];
    Point2f outputQuad[4];
    // double maxArea = 0.0;
    Rect boundRect;
    vector<vector<Point> > contours;
    outputQuad[0] = Point2f( 0,0 );
    outputQuad[1] = Point2f( 200,0);
    outputQuad[2] = Point2f( 200,200);
    outputQuad[3] = Point2f( 0,200  );

    inputTPl = imread( argv[1]);
    cvtColor(inputTPl, imageTPL_GRAY, COLOR_BGR2GRAY);

    vector<Point> maxMserTPL = maxMser(&imageTPL_GRAY);
    RotatedRect rect = minAreaRect(maxMserTPL);    
    Point2f points[4];
    rect.points(points);

    Mat M = getPerspectiveMatrix(points,rect.size);
    Mat normalizedImage = normalizeImage(&imageTPL_GRAY,&M,rect.size.width);
    imshow("M", normalizedImage);
    waitKey(0);

    imageTPL = normalizedImage;
    
    // MSER()(imageTPL_GRAY, contours);

    // vector<vector<Point> > contours_poly( contours.size() );
    // for( int i = 0; i < contours.size(); i++ )
    // { 
    //     double area = contourArea(contours[i]);
    //     if(area > maxArea) {
    //         maxArea = area;
    //         approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
    //         boundRect = boundingRect( Mat(contours_poly[i]) );
    //         drawContours(drawing, contours, i, Scalar(255, 255, 255), CV_FILLED);
    //     }
    // }

//+++++++++++++++++++++++++++++++++++++++++
// Normalizing Template into Square
// http://opencvexamples.blogspot.com/2014/01/perspective-transform.html

    // imageTPL_GRAY = imageTPL_GRAY(boundRect);

    // inputQuad[0] = Point2f( 0,0 );
    // inputQuad[1] = Point2f( imageTPL_GRAY.cols,0);
    // inputQuad[2] = Point2f( imageTPL_GRAY.cols,imageTPL_GRAY.rows);
    // inputQuad[3] = Point2f( 0,imageTPL_GRAY.rows);

    // lambda = getPerspectiveTransform( inputQuad, outputQuad );
    // warpPerspective(imageTPL_GRAY,imageTPL,lambda,drawing.size() );
 
    imshow("Input",imageTPL_GRAY);
    imshow("Template",imageTPL);
 
    waitKey(0);

// END: Normalizing Template into Square
//+++++++++++++++++++++++++++++++++++++++++

    vector<Point> normalizedMser = maxMser(&imageTPL);

    Features featuresTPL = extractFeature(&normalizedMser);

    cout << "PRINTING FROM MAIN: " << featuresTPL.numberOfHoles << "\t"
                            << featuresTPL.convexHullAreaRate << "\t"
                            << featuresTPL.minRectAreaRate << "\t"
                            << featuresTPL.skeletLengthRate << "\t"
                            << featuresTPL.contourAreaRate << endl;


// CREATING A TEMPLATE
//===================================================

    VideoCapture cap(0);
    if(!cap.isOpened()){
        return -1;
    }

    namedWindow("Final",1);

    while(true){
        Mat inputSCN, imageSCN, imageSCN_GRAY;
        cap >> inputSCN;
        mser(inputSCN, "MSER TEST", "logoT.jpg");
        // cvtColor(inputSCN, imageSCN_GRAY, COLOR_BGR2GRAY);
        // imageSCN = processImage(inputSCN, featuresTPL);
        // imshow("Final",imageSCN);
        // waitKey(0);
        if(waitKey(30) >= 0) break;
    }

    return 0;
}