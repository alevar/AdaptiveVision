// BENEFITS OF USING MSER
// Could be used to detect multiple predefined templates at the same time(simultaneously)
// Autonomous machines being able to respond to a different number of stimuli

/*==================================================
==================== TEMPLATE CLASS ================
====================================================
===== Class intended for creation of Templates =====
===== Templates are used against the incoming  =====
===== images, to identify similar objects	   =====
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

#include "Template.h"
#include "MatchMSER.h"
#include "Canvas.h"

using namespace std;
using namespace cv;

Template::Template(){
	this->outputQuad[0] = Point2f( 0,0 );
    this->outputQuad[1] = Point2f( 200,0);
    this->outputQuad[2] = Point2f( 200,200);
    this->outputQuad[3] = Point2f( 0,200  );
    this->lambda = Mat::zeros( 200, 200, this->imageTPL_GRAY.type() );
}

Template::Template(Mat image) {

    MatchMSER match;

	this->outputQuad[0] = Point2f( 0,0 );
    this->outputQuad[1] = Point2f( 200,0);
    this->outputQuad[2] = Point2f( 200,200);
    this->outputQuad[3] = Point2f( 0,200  );
    this->lambda = Mat::zeros( 200, 200, this->imageTPL_GRAY.type() );

    this->inputTPL = image;
    cv::cvtColor(inputTPL, imageTPL_GRAY, COLOR_BGR2GRAY);

    namedWindow("TEMPLATE CALIBRATION", WINDOW_NORMAL);
    createTrackbar("Max Area", "TEMPLATE CALIBRATION", &this->maxArea, 100);
    createTrackbar("Diversity", "TEMPLATE CALIBRATION", &this->diversity, 1000);

    while(true){



        this->maxMserTPL = Template::maxMser(&imageTPL_GRAY);
        this->rect = cv::minAreaRect(maxMserTPL);
        rect.points(points);

        this->M = Template::getPerspectiveMatrix(this->points,this->rect.size);
        this->imageTPL = Template::normalizeImage(&imageTPL_GRAY,&this->M,this->rect.size.width);

        drawing1 = Mat::zeros( imageTPL.cols+20,imageTPL.rows+20, CV_8UC3 );
        bitwise_not(drawing1,drawing1);
        cvtColor(drawing1, drawing1, CV_BGR2GRAY);

        Rect *roi = new Rect( cv::Point( 10, 10 ), imageTPL.size() );
        imageTPL.copyTo( drawing1( *roi ) );  

        normalizedMser = maxMser(&drawing1);

        drawing2 = mserToMat(&normalizedMser);

        erode(drawing2, drawing2, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
        dilate(drawing2, drawing2, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
        dilate(drawing2, drawing2, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
        erode(drawing2, drawing2, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));


        // MATCHING TO THE ORIGINAL
        match.setTemplate(&drawing1);
        match.setParams(this->maxArea, this->diversity);
        Mat testImage = image.clone();
        Mat imageMatch = match.findMatch(testImage);

        Mat testTPL = drawing2.clone();

        // cvtColor(imageMatch,imageMatch,CV_BGR2GRAY);
        // cvtColor(drawing2,drawing2,CV_BGR2GRAY);

        vector<Mat> testCanvas = {imageMatch,testTPL};

        // cout << "HELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLD" << endl;

        Canvas canvas(testCanvas);
        // cout << "4 HELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLD" << endl;
        Mat final = canvas.getMat();

        // cout << "5 HELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLD" << endl;

        // imshow("TESTING MSER WITHIN TEMPLATE", imageMatch);

        imshow("TEMPLATE CALIBRATION",final);

        if(waitKey(30) >= 0){
            
            // destroyWindow("HELLO MATCH HSV");
            delete roi;
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

    // cvReleaseImage(&imageTPL);
}

Mat Template::mserToMat(vector<Point> *mser)
{
    minX = min_element(mser->begin(), mser->end(), [] (Point &p1, Point &p2) { return p1.x < p2.x; })[0].x;
    minY = min_element(mser->begin(), mser->end(), [] (Point &p1, Point &p2) { return p1.y < p2.y; })[0].y;
    maxX = max_element(mser->begin(), mser->end(), [] (Point &p1, Point &p2) { return p1.x < p2.x; })[0].x;
    maxY = max_element(mser->begin(), mser->end(), [] (Point &p1, Point &p2) { return p1.y < p2.y; })[0].y;

    Mat *color = new Mat(maxY - minY, maxX - minX, CV_8UC3);
    
    for_each(mser->begin(), mser->end(), [&] (Point &p) 
    {
        newPoint = Point(p.x - minX, p.y - minY);
        line(*color, newPoint, newPoint, colors.WHITE);
    });    
    cvtColor(*color, gray, CV_BGRA2GRAY);
    
    delete color;
    return gray;
}

Mat Template::getPerspectiveMatrix(Point2f points[], Size2f size){
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
    
    return getPerspectiveTransform(src, dst);
}

Mat Template::normalizeImage(Mat *image, Mat *M ,float size){
    Mat dst(Size(size, size), image->type());
    cv::warpPerspective(*image, dst, *M, dst.size(), INTER_LINEAR, BORDER_DEFAULT, Scalar(1, 1, 1));

    return dst;
}

vector<Point> Template::maxMser(Mat *gray)
{

    double newMaxArea = maxArea/(double)1000;
    double newDiversity = diversity/(double)1000;

    MSER mser(21, (int)(newMaxArea*gray->cols*gray->rows), (int)(newDiversity*gray->cols*gray->rows), 1, 0.7);

    // Template::detectRegions(*gray, this->msers);
    mser(*gray, msers);

    if (this->msers.size() == 0){
        return vector<Point>();
    }
    
    vector<Point> mserF = max_element(this->msers.begin(), this->msers.end(), [] (vector<Point> &m1, vector<Point> &m2) {
        return m1.size() < m2.size();
    })[0];
    
    return mserF;
}

void Template::detectRegions(Mat &gray, vector<vector<Point> > & vector){
    
}

Mat Template::getTemplate(){
    return this->imageTPL;
}

Template::~Template() {
}