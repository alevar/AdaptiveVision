#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/features2d/features2d.hpp" 
#include <iostream>
#include <stdio.h>     

#include <stdarg.h> 
  
using namespace std;  
using namespace cv;  

void showCanvas(char* title, vector<Mat> matV) {

    Mat img;

    Mat DispImage;

    int size;
    int i;
    int m, n;
    int x, y;
 
    int width, height;

    float scale;
    int max;

    if(matV.size() <= 0) {
        printf("Number of arguments too small....\n");
        return;
    }
    else if(matV.size() > 12) {
        printf("Number of arguments too large....\n");
        return;
    }
    // Determine the size of the image, 0    // and the number of rows/cols 
    // from number of arguments 
    else if (matV.size() == 1) {
        width = height = 1;
        size = 300;
    }
    else if (matV.size() == 2) {
        width = 2; height = 1;
        size = 300;
    }
    else if (matV.size() == 3 || matV.size() == 4) {
        width = 2; height = 2;
        size = 300;
    }
    else if (matV.size() == 5 || matV.size() == 6) {
        width = 3; height = 2;
        size = 200;
    }
    else if (matV.size() == 7 || matV.size() == 8) {
        width = 4; height = 2;
        size = 200;
    }
    else {
        width = 4; height = 3;
        size = 150;
    }

    // DispImage = cvCreateImage( cvSize(100 + size*width, 60 + size*height), 8, 3 );
    Mat canvas((100 + size*width),(60 + size*height),CV_8U,cvScalar(0));
    cout << canvas.channels() << endl;
    // Mat canvas = Mat::zeros( (100 + size*width), (60 + size*height), 8);
    // cvtColor(canvas,canvas,CV_BGR2GRAY);


    for (i = 0, m = 20, n = 20; i < matV.size(); i++, m += (20 + size)) {

        img = matV[i];

        cvtColor( img, img, CV_BGR2GRAY );

        // if(img == 0) {
        //     printf("Invalid arguments");
        //     // cvReleaseImage(&DispImage);
        //     return;
        // }

        x = img.cols;
        y = img.rows;

        max = (x > y)? x: y;

        scale = (float) ( (float) max / size );

        if( i % width == 0 && m!= 20) {
            m = 20;
            n+= 20 + size;
        }

        // cvSetImageROI(DispImage, cvRect(m, n, (int)( x/scale ), (int)( y/scale )));
        Rect roi(m, n, (int)( x/scale ), (int)( y/scale ));
        Mat image_roi = canvas(roi);

        imshow("HIIII1", img);

        cout << img.channels() << "_" << canvas.channels() << endl;

        resize(img, image_roi, image_roi.size());

        imshow("HIIII", canvas);
        waitKey(0);

        // cvResetImageROI(DispImage);
    }

    namedWindow( title, 1 );
    imshow( title, canvas);

    waitKey();
    destroyWindow(title);

    // cvReleaseImage(&DispImage);
}

int main()  
{

    Mat img1 = imread("logoT.jpg");
    Mat img2 = imread("clc.jpg");
    Mat img3 = imread("clc2.jpg");

    Mat img4 = img1.clone();

    vector<Mat> imM = {img1,img2,img3};

    showCanvas("Image", imM);

}