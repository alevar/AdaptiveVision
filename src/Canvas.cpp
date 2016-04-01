/*=====================================
============= CLASS CANVAS ============
=======================================
==== Clas for creating a canvas of ====
======= many individual images ========
=====================================*/

#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdarg.h>

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

#include "Canvas.h"

using namespace std;
using namespace cv;

Canvas::Canvas(){
}

Canvas::Canvas(vector<Mat> matV){

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

    // canvas = cvCreateImage( cvSize(100 + size*width, 60 + size*height), 8, 3 );
    cout <<matV[0].type() << "_" << matV[0].channels()<<endl;
    Mat canvas((100 + size*width),(60 + size*height),CV_8UC3, CV_RGB(1,1,1));

    Mat image_roi;

    for (i = 0, m = 20, n = 20; i < matV.size(); i++, m += (20 + size)) {

        cout << "1 BEEEEEEEEEEGGIIIN" << endl;

        img = matV[i];

        cout << "Channels: " << img.channels() << "_" << canvas.channels() << endl;
        cout << "Type: " << img.type() << "_" << canvas.type() << endl;

        if(img.channels() != 3){
            cvtColor( img, img, CV_GRAY2BGR );
        }
        if(img.type() != 16){
            img.convertTo(img,CV_8UC3);
        }

        cout << "Channels: " << img.channels() << "_" << canvas.channels() << endl;
        cout << "Type: " << img.type() << "_" << canvas.type() << endl;
        cout << "2 BEEEEEEEEEEGGIIIN" << endl;

        imshow("IMAGE", img);
        waitKey(0);

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
        cout << " 3 BEEEEEEEEEEGGIIIN" << endl;

        // cvSetImageROI(DispImage, cvRect(m, n, (int)( x/scale ), (int)( y/scale )));
        Rect roi(m, n, (int)( x/scale ), (int)( y/scale ));
        cout << "4 BEEEEEEEEEEGGIIIN" << endl;
        image_roi = canvas(*roi);
        cout << "5 BEEEEEEEEEEGGIIIN" << endl;        

        resize(img, image_roi, image_roi.size());
        

        imshow("HIIII", canvas);
        waitKey(0);
        cout << "1 HELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLD" << endl;
    }

    cout << "2 HELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLD" << endl;

    this->resultCanvas = canvas.clone();

    cout << "3 HELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLDHELLO WORLD" << endl;

}

void Canvas::build(std::vector<Mat> matV){

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

    // canvas = cvCreateImage( cvSize(100 + size*width, 60 + size*height), 8, 3 );
    Mat canvas((100 + size*width),(60 + size*height),CV_8U,cvScalar(0));

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

        resize(img, image_roi, image_roi.size());
    }

    this->resultCanvas = canvas.clone();

}

void Canvas::show(){

}

Mat Canvas::getMat(){
    return this->resultCanvas;
}

Canvas::~Canvas() {
    
}