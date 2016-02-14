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

Histogram::Histogram(Mat Image) {
    this->plot = Image;
    this->calcHis();
}

void Histogram::calcHis(){

    for (int i = 0; i < this->plot.rows; i++)
    {
        for (int j = 0; j < this->plot.cols; j++)
        {
            Vec3b intensity = this->plot.at<Vec3b>(Point(j, i));
            int Red = intensity.val[0];
            int Green = intensity.val[1];
            int Blue = intensity.val[2];
            this->HistR[Red] = this->HistR[Red]+1;
            this->HistB[Blue] = this->HistB[Blue]+1;
            this->HistG[Green] = this->HistG[Green]+1;
        }
    }

    for (int i = 0; i < 256; i=i+2)
    {
        line(this->HistPlotR, Point(i, 500), Point(i, 500-this->HistR[i]), Scalar(0, 0, 255),1,8,0);
        line(this->HistPlotG, Point(i, 500), Point(i, 500-this->HistG[i]), Scalar(0, 255, 0),1,8,0);
        line(this->HistPlotB, Point(i, 500), Point(i, 500-this->HistB[i]), Scalar(255, 0, 0),1,8,0);
    }
}

void Histogram::toHistogram(Mat){

}

void Histogram::showHist(){

    namedWindow("Red Histogram");
    namedWindow("Green Histogram");
    namedWindow("Blue Histogram");
    imshow("Red Histogram", this->HistPlotR);
    imshow("Green Histogram", this->HistPlotG);
    imshow("Blue Histogram", this->HistPlotB);
    waitKey(0);

}

Histogram::~Histogram() {
}
