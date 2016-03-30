/*==================================================
=================== MATCH MSER CLASS ===============
====================================================
===== Class intended for identification of     =====
===== Matches between Templates and the input  =====
==================================================*/

#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <arpa/inet.h>
#include <stdlib.h>

#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc_c.h"

#include "MatchMSER.h"
#include "Template.h"

using namespace std;
using namespace cv;

MatchMSER::MatchMSER(){
	// this->mserDetector(Params.delta, Params.minArea, Params.maxArea,Params.maxVariation,Params.minDiversity,
			// Params.maxEvolution,Params.areaThreshold, Params.minMargin, Params.edgeBlurSize);
}

MatchMSER::MatchMSER(Mat *imgTPL){
	this->imgTPL = imgTPL;
	// this->mserDetector(Params.delta, Params.minArea, Params.maxArea,Params.maxVariation,Params.minDiversity,
			// Params.maxEvolution,Params.areaThreshold, Params.minMargin, Params.edgeBlurSize);
}

MatchMSER::MatchMSER(Mat *imgTPL, Mat *imgMAT){
	this->imgTPL = imgTPL;
	this->imgMAT = imgMAT;
	// this->mserDetector(Params.delta, Params.minArea, Params.maxArea,Params.maxVariation,Params.minDiversity,
			// Params.maxEvolution,Params.areaThreshold, Params.minMargin, Params.edgeBlurSize);
}

void MatchMSER::setTemplate(Mat *imgTPL){
	this->imgTPL = imgTPL;
}

void MatchMSER::setImage(Mat *imgMAT){
	this->imgMAT = imgMAT;
}
void MatchMSER::set(Mat *imgTPL, Mat *imgMAT){
	this->imgTPL = imgTPL;
	this->imgMAT = imgMAT;
}

Mat MatchMSER::findMatch(Mat imageMAT){
    this->normalizedMser = MatchMSER::maxMser(this->imgTPL);
    this->featuresTPL = MatchMSER::extractFeature(&normalizedMser);
	Mat matched = MatchMSER::processImage(imageMAT);
	return matched;
}

// void MatchMSER::setParams(int *delta,int *minArea,int *maxArea,double *maxVariation,double *minDiversity,
// 							int *maxEvolution,double *areaThreshold,double *minMargin,int *edgeBlurSize){

// 	this->params.delta 			= 	*delta;
// 	this->params.minArea 		= 	*minArea;
// 	this->params.maxArea 		= 	*maxArea;
// 	this->params.maxVariation 	= 	*maxVariation;
// 	this->params.minDiversity 	= 	*minDiversity;
// 	this->params.maxEvolution 	= 	*maxEvolution;
// 	this->params.areaThreshold 	= 	*areaThreshold;
// 	this->params.minMargin 		= 	*minMargin;
// 	this->params.edgeBlurSize 	= 	*edgeBlurSize;

// 	this->mserDetector(*delta,*minArea,*maxArea,*maxVariation,*minDiversity,*maxEvolution,*areaThreshold,*minMargin,*edgeBlurSize);
// }

// FUNCTIONS FOR CALCULATING MSERS AND FINDING MATCHES

void MatchMSER::detectRegions(Mat &gray, vector<vector<Point> > & vectorX){
    MatchMSER::mserDetector(gray, vectorX);
}

Mat MatchMSER::mserToMat(vector<Point> *mser)
{
    int minX = min_element(mser->begin(), mser->end(), [] (Point &p1, Point &p2) { return p1.x < p2.x; })[0].x;
    int minY = min_element(mser->begin(), mser->end(), [] (Point &p1, Point &p2) { return p1.y < p2.y; })[0].y;
    int maxX = max_element(mser->begin(), mser->end(), [] (Point &p1, Point &p2) { return p1.x < p2.x; })[0].x;
    int maxY = max_element(mser->begin(), mser->end(), [] (Point &p1, Point &p2) { return p1.y < p2.y; })[0].y;

    Mat color(maxY - minY, maxX - minX, CV_8UC3);
    
    for_each(mser->begin(), mser->end(), [&] (Point &p) 
    {
        Point newPoint = Point(p.x - minX, p.y - minY);
        line(color, newPoint, newPoint, this->colors.WHITE);
    });    
    Mat gray;
    cvtColor(color, gray, CV_BGRA2GRAY);
    
    return gray;
}

int MatchMSER::findHoles(Mat *img){
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

int MatchMSER::skeletLength(Mat *mserImg)
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

double MatchMSER::contourAreas(Mat *img)
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

Features MatchMSER::extractFeature(vector<Point> *mser){

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

bool MatchMSER::matchTemplate(Features featuresTPL, Features featuresMSER)
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

double MatchMSER::distace(Features *featuresTPL, Features *featuresMSER)
{
    return 
    (featuresTPL->numberOfHoles == featuresMSER->numberOfHoles ? 1 : 10) * 
    (log(fabs(featuresTPL->convexHullAreaRate - featuresMSER->convexHullAreaRate)) + 
     log(fabs(featuresTPL->minRectAreaRate - featuresMSER->minRectAreaRate)) + 
     log(fabs(featuresTPL->skeletLengthRate - featuresMSER->skeletLengthRate)) + 
     log(fabs(featuresTPL->contourAreaRate - featuresMSER->contourAreaRate)));
}

Mat MatchMSER::processImage(Mat imageMAT){
    Mat gray;
    cvtColor(imageMAT, gray, CV_BGRA2GRAY);
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
                
                double tmp = distance(&featuresTPL, &featuresMSER);
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
        rectangle(imageMAT, bound, this->colors.GREEN, 3);
    }

    // if (msers.size() == 0) { 
    //     return image; 
    // };

    return imageMAT;
}

vector<Point> MatchMSER::maxMser(Mat *gray)
{
    vector<vector<Point> > msers;

    detectRegions (*gray, msers);

    if (msers.size() == 0) return vector<Point>();
    
    vector<Point> mser = max_element(msers.begin(), msers.end(), [] (vector<Point> &m1, vector<Point> &m2) {
        return m1.size() < m2.size();
    })[0];
    
    return mser;
}

Mat MatchMSER::getPerspectiveMatrix(Point2f points[], Size2f size)
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

Mat MatchMSER::normalizeImage(Mat *image, Mat *M ,float size)
{
    Mat dst(Size(size, size), image->type());
    warpPerspective(*image, dst, *M, dst.size(), INTER_LINEAR, BORDER_DEFAULT, Scalar(1, 1, 1));

    return dst;
}

MatchMSER::~MatchMSER() {
}