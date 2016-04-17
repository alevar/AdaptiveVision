
    #include <cv.h>
    #include <highgui.h>
    #include <math.h>
    #include <unistd.h>
    #include <getopt.h>
    #include <iostream>


void MyDistortPoints(const std::vector<cv::Point2d> & src, std::vector<cv::Point2d> & dst, 
                     const cv::Mat & cameraMatrix, const cv::Mat & distorsionMatrix)
{
  dst.clear();
  double fx = cameraMatrix.at<double>(0,0);
  double fy = cameraMatrix.at<double>(1,1);
  double ux = cameraMatrix.at<double>(0,2);
  double uy = cameraMatrix.at<double>(1,2);

  double k1 = distorsionMatrix.at<double>(0, 0);
  double k2 = distorsionMatrix.at<double>(0, 1);
  double p1 = distorsionMatrix.at<double>(0, 2);
  double p2 = distorsionMatrix.at<double>(0, 3);
  double k3 = distorsionMatrix.at<double>(0, 4);
  //BOOST_FOREACH(const cv::Point2d &p, src)
  for (unsigned int i = 0; i < src.size(); i++)
  {
    const cv::Point2d & p = src[i];
    double x = p.x;
    double y = p.y;
    double xCorrected, yCorrected;
    //Step 1 : correct distorsion
    {     
      double r2 = x*x + y*y;
      //radial distorsion
      xCorrected = x * (1. + k1 * r2 + k2 * r2 * r2 + k3 * r2 * r2 * r2);
      yCorrected = y * (1. + k1 * r2 + k2 * r2 * r2 + k3 * r2 * r2 * r2);

      //tangential distorsion
      //The "Learning OpenCV" book is wrong here !!!
      //False equations from the "Learning OpenCv" book below :
      //xCorrected = xCorrected + (2. * p1 * y + p2 * (r2 + 2. * x * x)); 
      //yCorrected = yCorrected + (p1 * (r2 + 2. * y * y) + 2. * p2 * x);
      //Correct formulae found at : http://www.vision.caltech.edu/bouguetj/calib_doc/htmls/parameters.html
      xCorrected = xCorrected + (2. * p1 * x * y + p2 * (r2 + 2. * x * x));
      yCorrected = yCorrected + (p1 * (r2 + 2. * y * y) + 2. * p2 * x * y);
    }
    //Step 2 : ideal coordinates => actual coordinates
    {
      xCorrected = xCorrected * fx + ux;
      yCorrected = yCorrected * fy + uy;
    }
    dst.push_back(cv::Point2d(xCorrected, yCorrected));
  }

}

void MyDistortPoints(const std::vector<cv::Point2d> & src, std::vector<cv::Point2d> & dst, 
                     const cv::Matx33d & cameraMatrix, const cv::Matx<double, 1, 5> & distorsionMatrix)
{
  cv::Mat cameraMatrix2(cameraMatrix);
  cv::Mat distorsionMatrix2(distorsionMatrix);
  return MyDistortPoints(src, dst, cameraMatrix2, distorsionMatrix2);
}

void TestDistort()
{
  cv::Matx33d cameraMatrix = 0.;
  {
    //cameraMatrix Init
    double fx = 1000., fy = 950.;
    double ux = 324., uy = 249.;
    cameraMatrix(0, 0) = fx;
    cameraMatrix(1, 1) = fy;
    cameraMatrix(0, 2) = ux;
    cameraMatrix(1, 2) = uy;
    cameraMatrix(2, 2) = 1.;
  }

  cv::Matx<double, 1, 5> distorsionMatrix;
  {
    //distorsion Init
    const double k1 = 0.5, k2 = -0.5, k3 = 0.000005, p1 = 0.07, p2 = -0.05;

    distorsionMatrix(0, 0) = k1;
    distorsionMatrix(0, 1) = k2;
    distorsionMatrix(0, 2) = p1;
    distorsionMatrix(0, 3) = p2;
    distorsionMatrix(0, 4) = k3;
  }

  std::vector<cv::Point2d> distortedPoints;
  std::vector<cv::Point2d> undistortedPoints;
  std::vector<cv::Point2d> redistortedPoints;
  distortedPoints.push_back(cv::Point2d(324., 249.));// equals to optical center
  distortedPoints.push_back(cv::Point2d(340., 200));
  distortedPoints.push_back(cv::Point2d(785., 345.));
  distortedPoints.push_back(cv::Point2d(0., 0.));
  cv::undistortPoints(distortedPoints, undistortedPoints, cameraMatrix, distorsionMatrix);  
  MyDistortPoints(undistortedPoints, redistortedPoints, cameraMatrix, distorsionMatrix);
  cv::undistortPoints(redistortedPoints, undistortedPoints, cameraMatrix, distorsionMatrix);  

  //Poor man's unit test ensuring we have an accuracy that is better than 0.001 pixel
  for (unsigned int i = 0; i < undistortedPoints.size(); i++)
  {
    cv::Point2d dist = redistortedPoints[i] - distortedPoints[i];
    double norm = sqrt(dist.dot(dist));
    std::cout << "norm = " << norm << std::endl;
    assert(norm < 1E-3);
  }
}

int main(){
  TestDistort();
  return 0;
}