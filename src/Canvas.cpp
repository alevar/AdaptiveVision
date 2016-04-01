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

Canvas::Canvas(int nArgs, ...){

    if(nArgs <= 0) {
        printf("Number of arguments too small....\n");
        return;
    }
    else if(nArgs > 12) {
        printf("Number of arguments too large....\n");
        return;
    }
    // Determine the size of the image, 0    // and the number of rows/cols 
    // from number of arguments 
    else if (nArgs == 1) {
        width = height = 1;
        size = 300;
    }
    else if (nArgs == 2) {
        width = 2; height = 1;
        size = 300;
    }
    else if (nArgs == 3 || nArgs == 4) {
        width = 2; height = 2;
        size = 300;
    }
    else if (nArgs == 5 || nArgs == 6) {
        width = 3; height = 2;
        size = 200;
    }
    else if (nArgs == 7 || nArgs == 8) {
        width = 4; height = 2;
        size = 200;
    }
    else {
        width = 4; height = 3;
        size = 150;
    }

    canvas = cvCreateImage( cvSize(100 + size*width, 60 + size*height), 8, 3 );

    va_list args;
    va_start(args, nArgs);

    for (i = 0, m = 20, n = 20; i < nArgs; i++, m += (20 + size)) {

        img = va_arg(args, IplImage*);

        if(img == 0) {
            printf("Invalid arguments");
            cvReleaseImage(&canvas);
            return;
        }

        x = img->width;
        y = img->height;

        max = (x > y)? x: y;

        scale = (float) ( (float) max / size );

        if( i % width == 0 && m!= 20) {
            m = 20;
            n+= 20 + size;
        }

        cvSetImageROI(canvas, cvRect(m, n, (int)( x/scale ), (int)( y/scale )));

        cvResize(img, canvas);

        cvResetImageROI(canvas);
    }

}

void Canvas::build(int nArgs, ...){

    if(nArgs <= 0) {
        printf("Number of arguments too small....\n");
        return;
    }
    else if(nArgs > 12) {
        printf("Number of arguments too large....\n");
        return;
    }
    // Determine the size of the image, 0    // and the number of rows/cols 
    // from number of arguments 
    else if (nArgs == 1) {
        width = height = 1;
        size = 300;
    }
    else if (nArgs == 2) {
        width = 2; height = 1;
        size = 300;
    }
    else if (nArgs == 3 || nArgs == 4) {
        width = 2; height = 2;
        size = 300;
    }
    else if (nArgs == 5 || nArgs == 6) {
        width = 3; height = 2;
        size = 200;
    }
    else if (nArgs == 7 || nArgs == 8) {
        width = 4; height = 2;
        size = 200;
    }
    else {
        width = 4; height = 3;
        size = 150;
    }

    canvas = cvCreateImage( cvSize(100 + size*width, 60 + size*height), 8, 3 );

    va_list args;
    va_start(args, nArgs);

    for (i = 0, m = 20, n = 20; i < nArgs; i++, m += (20 + size)) {

        img = va_arg(args, IplImage*);

        if(img == 0) {
            printf("Invalid arguments");
            cvReleaseImage(&canvas);
            return;
        }

        x = img->width;
        y = img->height;

        max = (x > y)? x: y;

        scale = (float) ( (float) max / size );

        if( i % width == 0 && m!= 20) {
            m = 20;
            n+= 20 + size;
        }

        cvSetImageROI(canvas, cvRect(m, n, (int)( x/scale ), (int)( y/scale )));

        cvResize(img, canvas);

        cvResetImageROI(canvas);
    }

}

void Canvas::show(){

}

Mat Canvas::getMat(){
    return cvarrToMat(this->canvas);
}

Canvas::~Canvas() {
    
}