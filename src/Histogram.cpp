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

    convertRGB2HSV(this->imageRGB);
    Mat imageHSVT = imageRGB.clone();
    cvtColor(imageHSVT, imageHSVT, COLOR_BGR2HSV);

    for (int i = 0; i < imageHSVT.rows; i++)
    {
        for (int j = 0; j < imageHSVT.cols; j++)
        {
            intensityHSV = imageHSVT.at<Vec3b>(Point(j, i));
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
    cout << "PRINTING VECTORS OF HSV VALUES:::::::::: ";
    for (int i = 0; i < vecH.size(); ++i)
    {
        cout << vecH[i] << ", ";
    }
    cout << endl;
    vecH.erase (vecH.begin()+255,vecH.end());
    vecH.erase (vecH.begin(),vecH.begin()+1);

    vector<int> vecS(this->HistS, this->HistS + sizeof this->HistS / sizeof this->HistS[0]);    
    cout << "PRINTING VECTORS OF HSV VALUES:::::::::: ";
    for (int i = 0; i < vecS.size(); ++i)
    {
        cout << vecS[i] << ", ";
    }
    cout << endl;
    vecS.erase (vecS.begin()+255,vecS.end());
    vecS.erase (vecS.begin(),vecS.begin()+1);

    vector<int> vecV(this->HistV, this->HistV + sizeof this->HistV / sizeof this->HistV[0]);
    cout << "PRINTING VECTORS OF HSV VALUES:::::::::: ";
    for (int i = 0; i < vecV.size(); ++i)
    {
        cout << vecV[i] << ", ";
    }
    cout << endl;
    vecV.erase (vecV.begin()+255,vecV.end());
    vecV.erase (vecV.begin(),vecV.begin()+1);

    this->result={};
// STARTING ON NEW FUNCTIONALITY

    // auto biggestH = max_element(vecH.begin(), vecH.end());
    // int gH = (int)*biggestH;
    // int posH = distance(vecH.begin(), biggestH)+1;

    // auto biggestS = max_element(vecS.begin(), vecS.end());
    // int gS = (int)*biggestS;
    // int posS = distance(vecS.begin(), biggestS)+1;

    // auto biggestV = max_element(vecV.begin(), vecV.end());
    // int gV = (int)*biggestV;
    // int posV = distance(vecV.begin(), biggestV)+1;

    // cout << "BIGGEST: " << (int)*biggestH << " " << (int)*biggestS << " " << (int)*biggestV << endl;
    // cout << "BIGGEST: " << (int)*biggestH << " " << (int)*biggestS << " " << (int)*biggestV << endl;

// ANOTHER TEST

    // this->result.push_back(1+distance( begin(vecH), find_if( begin(vecH), end(vecH), [](int x) { return x != 0; })));
    // reverse(begin(vecH),end(vecH));
    // this->result.push_back(1+vecH.size()-distance( begin(vecH), find_if( begin(vecH), end(vecH), [](int x) { return x != 0; })));

    // this->result.push_back(1+distance( begin(vecS), find_if( begin(vecS), end(vecS), [](int x) { return x != 0; })));
    // reverse(begin(vecS),end(vecS));
    // this->result.push_back(1+vecS.size()-distance( begin(vecS), find_if( begin(vecS), end(vecS), [](int x) { return x != 0; })));

    // this->result.push_back(1+distance( begin(vecV), find_if( begin(vecV), end(vecV), [](int x) { return x != 0; })));
    // reverse(begin(vecV),end(vecV));
    // this->result.push_back(1+vecV.size()-distance( begin(vecV), find_if( begin(vecV), end(vecV), [](int x) { return x != 0; })));


// ANOTHER TEST

    // this->result.push_back(1+distance( begin(vecH), find_if( begin(vecH), end(vecH), [biggestH](int x) { return x >= *biggestH/5; })));
    // reverse(begin(vecH),end(vecH));
    // this->result.push_back(1+vecH.size()-distance( begin(vecH), find_if( begin(vecH), end(vecH), [biggestH](int x) { return x >= (*biggestH/5); })));

    // this->result.push_back(1+distance( begin(vecS), find_if( begin(vecS), end(vecS), [biggestS](int x) { return x >= *biggestS/5; })));
    // reverse(begin(vecS),end(vecS));
    // this->result.push_back(1+vecS.size()-distance( begin(vecS), find_if( begin(vecS), end(vecS), [biggestS](int x) { return x >= (*biggestS/5); })));

    // this->result.push_back(1+distance( begin(vecV), find_if( begin(vecV), end(vecV), [biggestV](int x) { return x >= *biggestV/5; })));
    // reverse(begin(vecV),end(vecV));
    // this->result.push_back(1+vecV.size()-distance( begin(vecV), find_if( begin(vecV), end(vecV), [biggestV](int x) { return x >= (*biggestV/5); })));

// YET ANOTHER TEST

    // this->result.push_back(1+(std::upper_bound (begin(vecH), end(vecH), (gH/5))-begin(vecH)));
    // reverse(begin(vecH),end(vecH));
    // this->result.push_back(1+vecH.size()-(std::upper_bound (begin(vecH), end(vecH), (gH/5))-begin(vecH)));

    // this->result.push_back(1+(std::upper_bound (begin(vecS), end(vecS), (gS/5))-begin(vecS)));
    // reverse(begin(vecS),end(vecS));
    // this->result.push_back(1+vecS.size()-(std::upper_bound (begin(vecS), end(vecS), (gS/5))-begin(vecS)));

    // this->result.push_back(1+(std::upper_bound (begin(vecV), end(vecV), (gV/5))-begin(vecV)));
    // reverse(begin(vecV),end(vecV));
    // this->result.push_back(1+vecV.size()-(std::upper_bound (begin(vecV), end(vecV), (gV/5))-begin(vecV)));

    // cout << "BIGGEST: " << (int)*biggestH << " " << (int)*biggestS << " " << (int)*biggestV << endl;
    // cout << "BIGGEST: " << gH << " " << gS << " " << gV << endl;

// YET ANOTHER YET ANOTHER TEST

    vector<int> newTestH = {};
    for (int i = 0; i < vecH.size(); ++i){
        if(i<2 || i>vecH.size()-2){
            newTestH.push_back(vecH[i]);
        }
        else{

            int mean = (*prev(prev(vecH.begin()+i))+*prev(vecH.begin()+i)+*(vecH.begin()+i)+*next(vecH.begin()+i)+*next(next(vecH.begin()+i)))/5;
            newTestH.push_back(mean);
        }
    }

    auto biggestH = max_element(begin(newTestH), end(newTestH));
    int minThreshH = (int)((*biggestH*8)/100);

    this->result.push_back(1+distance( begin(newTestH), find_if( begin(newTestH), end(newTestH), [minThreshH](int x) { return x >= minThreshH; })));
    reverse(begin(newTestH),end(newTestH));
    this->result.push_back(1+newTestH.size()-distance( begin(newTestH), find_if( begin(newTestH), end(newTestH), [minThreshH](int x) { return x >= minThreshH; })));
    reverse(begin(newTestH),end(newTestH));

    vector<int> newTestS = {};
    for (int i = 0; i < vecS.size(); ++i){
        if(i<2 || i>vecS.size()-2){
            newTestS.push_back(vecS[i]);
        }
        else{

            int mean = (*prev(prev(vecS.begin()+i))+*prev(vecS.begin()+i)+*(vecS.begin()+i)+*next(vecS.begin()+i)+*next(next(vecS.begin()+i)))/5;
            newTestS.push_back(mean);
        }
    }

    auto biggestS = max_element(begin(newTestS), end(newTestS));
    int minThreshS = (int)((*biggestS*1)/100);

    this->result.push_back(1+distance( begin(newTestS), find_if( begin(newTestS), end(newTestS), [minThreshS](int x) { return x >= minThreshS; })));
    reverse(begin(newTestS),end(newTestS));
    this->result.push_back(1+newTestS.size()-distance( begin(newTestS), find_if( begin(newTestS), end(newTestS), [minThreshS](int x) { return x >= minThreshS; })));
    reverse(begin(newTestS),end(newTestS));

    vector<int> newTestV = {};
    for (int i = 0; i < vecV.size(); ++i){
        if(i<2 || i>vecV.size()-2){
            newTestV.push_back(vecV[i]);
        }
        else{

            int mean = (*prev(prev(vecV.begin()+i))+*prev(vecV.begin()+i)+*(vecV.begin()+i)+*next(vecV.begin()+i)+*next(next(vecV.begin()+i)))/5;
            newTestV.push_back(mean);
        }
    }

    auto biggestV = max_element(begin(newTestV), end(newTestV));
    int minThreshV = (int)((*biggestV*1)/100);

    this->result.push_back(1+distance( begin(newTestV), find_if( begin(newTestV), end(newTestV), [minThreshV](int x) { return x >= minThreshV; })));
    reverse(begin(newTestV),end(newTestV));
    this->result.push_back(1+newTestV.size()-distance( begin(newTestV), find_if( begin(newTestV), end(newTestV), [minThreshV](int x) { return x >= minThreshV; })));
    reverse(begin(newTestV),end(newTestV));


    // cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;

    // for (int i = 0; i < this->result.size(); ++i){
    //     cout << this->result[i] << endl;
    // }
    
    // cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;

// END HERE

    // auto biggestR = max_element(vecH.begin(), vecH.end());
    // int posR = distance(vecH.begin(), biggestR)+1;

    // auto biggestG = max_element(vecS.begin(), vecS.end());
    // int posG = distance(vecS.begin(), biggestG)+1;

    // auto biggestB = max_element(vecV.begin(), vecV.end());
    // int posB = distance(vecV.begin(), biggestB)+1;

    // Mat HSV(1,1, CV_8UC3, Scalar(posB,posG,posR));

    // cvtColor(HSV, HSV, COLOR_BGR2HSV);

    // cout << "FULL HSV" << HSV << endl;

    // Vec3b hsvVALS=HSV.at<Vec3b>(0,0);

    // this->result = {posR-25,posR+25,posG-20,posG+20,posB-20,posB+20};

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