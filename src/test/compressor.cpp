#include <string>
#include <iostream>
#include <map> 
#include <iterator>
#include <vector>
#include <fstream>

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

using namespace std;
using namespace cv;
 
int main() {

  VideoCapture source(0); // open the default camera
  if(!source.isOpened())  // check if we succeeded
  {
    return -1;
  }

  Mat image;
  source >> image;

  image = (image.reshape(0,1));
  // int  imgSize = image.total()*image.elemSize()

  unsigned char* test = image.data;
  const char * c = (const char*)test;
  string test2 = c;

  ofstream myfile ("test.txt");
  if (myfile.is_open())
  {
    myfile << test2;
    myfile.close();
  }
  else cout << "Unable to open file";
 
  return 0;
}