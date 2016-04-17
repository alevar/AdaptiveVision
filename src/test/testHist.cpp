// g++ -std=c++11 -o testHist testHist.cpp `pkg-config opencv --cflags --libs`

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

#define MAX_BIT     256

int Hue;
int Val;
int Sat;
Vec3b intensityHSV;

bool hisHSV;

int HistH[MAX_BIT+1] = {0};
int HistS[MAX_BIT+1] = {0};
int HistV[MAX_BIT+1] = {0};

Mat HistPlotH = Mat (500, 256, CV_8UC3, Scalar(0, 0, 0));
Mat HistPlotS = Mat (500, 256, CV_8UC3, Scalar(0, 0, 0));
Mat HistPlotV = Mat (500, 256, CV_8UC3, Scalar(0, 0, 0));

vector<int> mmHue;
vector<int> mmSat;
vector<int> mmVal;

void convertRGB2HSV(Mat* image){
    cvtColor(*image, *image, COLOR_BGR2HSV);
}

Mat cloned;

vector<int> calcHisHSV(Mat image){

    cloned = image.clone();

    hisHSV = true;

    // convertRGB2HSV(&cloned);

    for (int i = 0; i < cloned.rows; i++)
    {
        for (int j = 0; j < cloned.cols; j++)
        {
            intensityHSV = cloned.at<Vec3b>(Point(j, i));
            Hue = intensityHSV.val[0];
            Sat = intensityHSV.val[1];
            Val = intensityHSV.val[2];
            HistH[Hue] = HistH[Hue]+1;
            HistS[Sat] = HistS[Sat]+1;
            HistV[Val] = HistV[Val]+1;
        }
    }

    for (int i = 0; i < 256; i=i+2)
    {
        line(HistPlotH, Point(i, 500), Point(i, 500-HistH[i]), Scalar(0, 0, 255),1,8,0);
        line(HistPlotS, Point(i, 500), Point(i, 500-HistS[i]), Scalar(0, 255, 0),1,8,0);
        line(HistPlotV, Point(i, 500), Point(i, 500-HistV[i]), Scalar(255, 0, 0),1,8,0);
    }

    auto biggest = max_element(begin(HistH), end(HistH));
    int pos = distance(begin(HistH), biggest);

    int minThresh = (int)((*biggest*20)/100);

    int dist = *biggest;
    int compDist;
    int closest;

    for (int i = 0; i < MAX_BIT+1; ++i)
    {
        compDist = HistH[i]-minThresh;
        if(HistH[i]>minThresh && compDist<dist){
            dist = compDist;
            closest = i;
        }
    }

    if(pos+closest > MAX_BIT){
        mmHue = {closest,MAX_BIT};
    }
    else{
        mmHue = {closest,pos+closest};
    }

    biggest = max_element(begin(HistS), end(HistS));
    pos = distance(begin(HistS), biggest);

    minThresh = (int)((*biggest*20)/100);

    dist = *biggest;

    for (int i = 0; i < MAX_BIT+1; ++i)
    {
        compDist = HistS[i]-minThresh;
        if(HistS[i]>minThresh && compDist<dist){
            dist = compDist;
            closest = i;
        }
    }

    if(pos+closest > MAX_BIT){
        mmSat = {closest,MAX_BIT};
    }
    else{
        mmSat = {closest,pos+closest};
    }

    biggest = max_element(begin(HistV), end(HistV));
    pos = distance(begin(HistV), biggest);

    minThresh = (int)((*biggest*20)/100);

    dist = *biggest;

    for (int i = 0; i < MAX_BIT+1; ++i)
    {
        compDist = HistV[i]-minThresh;
        if(HistV[i]>minThresh && compDist<dist){
            dist = compDist;
            closest = i;
        }
    }

    if(pos+closest > MAX_BIT){
        mmVal = {closest,MAX_BIT};
    }
    else{
        mmVal = {closest,pos+closest};
    }

    vector<int> result = {};
    result.insert( result.end(), mmHue.begin(), mmHue.end() );
    result.insert( result.end(), mmSat.begin(), mmSat.end() );
    result.insert( result.end(), mmVal.begin(), mmVal.end() );


    // Mat M(1,1, CV_8UC3, Scalar(result[0],result[2],result[4]));
    // cout << "++++++++++++++______________++++++++++++++++++++++++" << endl;
    // cout << "RGB" << M << endl;
    // cout << "++++++++++++++______________++++++++++++++++++++++++" << endl;
    // // convertRGB2HSV(&M);
    // cvtColor(M, M, COLOR_BGR2HSV);
    // cout << "++++++++++++++______________++++++++++++++++++++++++" << endl;
    // cout << "HSV" << M << endl;
    // cout << "++++++++++++++______________++++++++++++++++++++++++" << endl;

    Mat MinHSV(1,1, CV_8UC3, Scalar(result[0],result[2],result[4]));
    cvtColor(MinHSV, MinHSV, COLOR_BGR2HSV);

    Mat MaxHSV(1,1, CV_8UC3, Scalar(result[1],result[3],result[5]));
    cvtColor(MaxHSV, MaxHSV, COLOR_BGR2HSV);

    Vec3b hsvMIN=MinHSV.at<Vec3b>(0,0);
    Vec3b hsvMAX=MaxHSV.at<Vec3b>(0,0);

    vector<int> resultHSVmM = {hsvMIN.val[0],hsvMIN.val[1],hsvMIN.val[2],hsvMAX.val[0],hsvMAX.val[1],hsvMAX.val[2]};

    cout << "++++++++++++++______________++++++++++++++++++++++++" << endl;
    for (int i = 0; i < resultHSVmM.size(); ++i)
    {
        cout << resultHSVmM[i] << endl;
    }
    cout << "++++++++++++++______________++++++++++++++++++++++++" << endl;

    return result;
}

int main( int argc, char** argv ){

    Mat inputSCN;

    VideoCapture cap(0);
    if(!cap.isOpened()){
        return -1;
    }

    namedWindow("MATCH",1);
    while(true){
      
      cap >> inputSCN;

      vector<int> hsvRes = calcHisHSV(inputSCN);

      imshow("MATCH",inputSCN);
      if(waitKey(30) >= 0){
            break;
        }
    }
    return 0;
}
