/*==================================================
==================================================*/

#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <arpa/inet.h>
#include <stdlib.h>
#include <algorithm>

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

    vector<int> testV(this->HistR, this->HistR + sizeof this->HistR / sizeof this->HistR[0]);

    testV.erase (testV.begin()+240,testV.end());
    testV.erase (testV.begin(),testV.begin()+15);

    auto biggest = max_element(testV.begin(), testV.end());
    int pos = distance(testV.begin(), biggest);
}

void Histogram::calcHisHSV(){

    hisHSV = true;

    // convertRGB2HSV(this->imageRGB);

    for (int i = 0; i < this->imageRGB.rows; i++)
    {
        for (int j = 0; j < this->imageRGB.cols; j++)
        {
            intensityHSV = this->imageRGB.at<Vec3b>(Point(j, i));
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

    vector<int> vecH(this->HistH, this->HistH + sizeof this->HistH / sizeof this->HistH[0]);
    vecH.erase (vecH.begin()+251,vecH.end());
    vecH.erase (vecH.begin(),vecH.begin()+1);

    vector<int> vecS(this->HistS, this->HistS + sizeof this->HistS / sizeof this->HistS[0]);
    vecS.erase (vecS.begin()+251,vecS.end());
    vecS.erase (vecS.begin(),vecS.begin()+1);

    vector<int> vecV(this->HistV, this->HistV + sizeof this->HistV / sizeof this->HistV[0]);
    vecV.erase (vecV.begin()+251,vecV.end());
    vecV.erase (vecV.begin(),vecV.begin()+1);

    this->result={};
// STARTING ON NEW FUNCTIONALITY

    this->result.push_back(1+distance( begin(vecH), find_if( begin(vecH), end(vecH), [](int x) { return x != 0; })));
    reverse(begin(vecH),end(vecH));
    this->result.push_back(5+vecH.size()-distance( begin(vecH), find_if( begin(vecH), end(vecH), [](int x) { return x != 0; })));

    this->result.push_back(1+distance( begin(vecS), find_if( begin(vecS), end(vecS), [](int x) { return x != 0; })));
    reverse(begin(vecS),end(vecS));
    this->result.push_back(5+vecS.size()-distance( begin(vecS), find_if( begin(vecS), end(vecS), [](int x) { return x != 0; })));

    this->result.push_back(1+distance( begin(vecV), find_if( begin(vecV), end(vecV), [](int x) { return x != 0; })));
    reverse(begin(vecV),end(vecV));
    this->result.push_back(5+vecV.size()-distance( begin(vecV), find_if( begin(vecV), end(vecV), [](int x) { return x != 0; })));

    // cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;

    // for (int i = 0; i < this->result.size(); ++i){
    //     cout << this->result[i] << endl;
    // }
    
    // cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;

// END HERE

    // auto biggestR = max_element(vecH.begin(), vecH.end());
    // int posR = distance(vecH.begin(), biggestR);

    // auto biggestG = max_element(vecS.begin(), vecS.end());
    // int posG = distance(vecS.begin(), biggestG);

    // auto biggestB = max_element(vecV.begin(), vecV.end());
    // int posB = distance(vecV.begin(), biggestB);

    // Mat HSV(1,1, CV_8UC3, Scalar(posB,posG,posR));

    // cvtColor(HSV, HSV, COLOR_BGR2HSV);

    // cout << "FULL HSV" << HSV << endl;

    // Vec3b hsvVALS=HSV.at<Vec3b>(0,0);

    // this->result = {hsvVALS.val[0],hsvVALS.val[1],hsvVALS.val[2]};

}

void Histogram::toHistogram(Mat){

}

// vector<int> Histogram::find20P(int[] arr){

//     auto biggest = max_element(begin(arr), end(arr));
//     int pos = distance(begin(arr), biggest);

//     int minThresh = (int)((*biggest*20)/100);

//     int distance = *biggest;
//     int compDist;
//     int closest;

//     for (int i = 0; i < MAX_BIT+1; ++i)
//     {
//         compDist = arr[i]-minThresh;
//         if(arr[i]>minThresh && compDist<distance){
//             distance = compDist;
//             closest = i;
//         }
//     }

//     vector<int> result = {closest,pos+closest};

//     return result;
// }

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
        imshow("Hue Histogram", this->HistPlotH);
        imshow("Sat Histogram", this->HistPlotS);
        imshow("Val Histogram", this->HistPlotV);
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

vector<int> Histogram::getVal(){
    // vector<int> result = {};
    // result.insert( result.end(), this->mmHue.begin(), this->mmHue.end() );
    // result.insert( result.end(), this->mmSat.begin(), this->mmSat.end() );
    // result.insert( result.end(), this->mmVal.begin(), this->mmVal.end() );

    return this->result;
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
    // imshow("HSV",this->imageHSV);
    // imshow("IMAGE IN HSV", image);
}