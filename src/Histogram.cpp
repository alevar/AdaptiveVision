/*==================================================
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

#include "Histogram.h"

using namespace std;
using namespace cv;

Histogram::Histogram(){
}

Histogram::Histogram(Mat image) {
    this->imageRGB = image;
    this->hisRGB = false;
    this->hisHSV = false;
}

void Histogram::calcHis(){

    hisRGB = true;

    for (int i = 0; i < this->imageRGB.rows; i++)
    {
        for (int j = 0; j < this->imageRGB.cols; j++)
        {
            this->intensityRGB = this->imageRGB.at<Vec3b>(Point(j, i));
            this->Red = this->intensityRGB.val[0];
            this->Green = this->intensityRGB.val[1];
            this->Blue = this->intensityRGB.val[2];
            this->HistR[this->Red] = this->HistR[this->Red]+1;
            this->HistB[this->Blue] = this->HistB[this->Blue]+1;
            this->HistG[this->Green] = this->HistG[this->Green]+1;
        }
    }

    for (int i = 0; i < 256; i=i+2)
    {
        line(this->HistPlotR, Point(i, 500), Point(i, 500-this->HistR[i]), Scalar(0, 0, 255),1,8,0);
        line(this->HistPlotG, Point(i, 500), Point(i, 500-this->HistG[i]), Scalar(0, 255, 0),1,8,0);
        line(this->HistPlotB, Point(i, 500), Point(i, 500-this->HistB[i]), Scalar(255, 0, 0),1,8,0);
    }
}

void Histogram::calcHisHSV(){

    hisHSV = true;

    convertRGB2HSV(this->plot);

    for (int i = 0; i < this->imageHSV.rows; i++)
    {
        for (int j = 0; j < this->imageHSV.cols; j++)
        {
            intensityHSV = this->imageHSV.at<Vec3b>(Point(j, i));
            this->Hue = this->intensityHSV.val[0];
            this->Sat = this->intensityHSV.val[1];
            this->Val = this->intensityHSV.val[2];
            this->HistH[this->Hue] = this->HistH[this->Hue]+1;
            this->HistS[this->Sat] = this->HistS[this->Sat]+1;
            this->HistV[this->Val] = this->HistV[this->Val]+1;
        }
    }

    for (int i = 0; i < 256; i=i+2)
    {
        line(this->HistPlotH, Point(i, 500), Point(i, 500-this->HistH[i]), Scalar(0, 0, 255),1,8,0);
        line(this->HistPlotS, Point(i, 500), Point(i, 500-this->HistS[i]), Scalar(0, 255, 0),1,8,0);
        line(this->HistPlotV, Point(i, 500), Point(i, 500-this->HistV[i]), Scalar(255, 0, 0),1,8,0);
    }
}

void Histogram::toHistogram(Mat){

}

void Histogram::showHist(){

    // Set boolean flags when calculating histograms
    // check for boolean flags when displaying histograms
    // if only rgb was done - display only those
    // if hsv was done display only that
    // if both were performed - display all

    if(hisRGB && !hisHSV){

        // namedWindow("Original Image");
        // namedWindow("Red Histogram");
        // namedWindow("Green Histogram");
        // namedWindow("Blue Histogram");
        // imshow("Original Image", this->imageRGB);
        imshow("Red Histogram", this->HistPlotR);
        imshow("Green Histogram", this->HistPlotG);
        imshow("Blue Histogram", this->HistPlotB);
        // waitKey(0);
        if(waitKey(30) >= 0){
            
            
        }

    }

    else if(!hisRGB && hisHSV){

        // namedWindow("Original Image");
        // namedWindow("Red Histogram");
        // namedWindow("Green Histogram");
        // namedWindow("Blue Histogram");
        // imshow("Original Image", this->imageHSV);
        imshow("Hue Histogram", this->HistPlotR);
        imshow("Sat Histogram", this->HistPlotG);
        imshow("Val Histogram", this->HistPlotB);
        // waitKey(0);
        if(waitKey(30) >= 0){
            
            
        }

    }

    else if(hisRGB && hisHSV){

        // namedWindow("Original Image");
        // namedWindow("Red Histogram");
        // namedWindow("Green Histogram");
        // namedWindow("Blue Histogram");
        // imshow("Original Image", this->imageRGB);
        imshow("Red Histogram", this->HistPlotR);
        imshow("Green Histogram", this->HistPlotG);
        imshow("Blue Histogram", this->HistPlotB);
        // waitKey(0);
        if(waitKey(30) >= 0){
            
            
        }

    }

    else{
        cout << "ERROR: No histogram has been previously calculated" << endl;
        exit(1);
    }
    

}

vector<int> Histogram::getMin(){

    return this->minValues;
}

vector<int> Histogram::getMax(){

    return this->maxValues;
}

Histogram::~Histogram() {
}

void Histogram::convertRGB2HSV(Mat image){
    cvtColor(image, this->imageHSV, COLOR_BGR2HSV);
}