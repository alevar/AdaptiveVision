// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".

#include <stdio.h>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/nonfree/nonfree.hpp"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

using namespace std;
using namespace cv;

class MyApp: public wxApp
{
public:
    virtual bool OnInit();
};
class MyFrame: public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
};
enum
{
    ID_Hello = 1
};
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_Hello,   MyFrame::OnHello)
    EVT_MENU(wxID_EXIT,  MyFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame( "Hello World", wxPoint(50, 50), wxSize(450, 340) );
    frame->Show( true );
    return true;
}
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, "&File" );
    menuBar->Append( menuHelp, "&Help" );
    SetMenuBar( menuBar );
    CreateStatusBar();
    SetStatusText( "Welcome to wxWidgets!" );
}
void MyFrame::OnExit(wxCommandEvent& event)
{
    Close( true );
}
void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox( "This is a wxWidgets' Hello world sample",
                  "About Hello World", wxOK | wxICON_INFORMATION );
}
void MyFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}



void Cv2WxImage(const Mat& cvImg, wxImage& wxImg)
{
  cv::Mat grayOutput, rgbOutput;
  cvImg.convertTo(grayOutput,CV_8U);
  cvtColor(grayOutput, rgbOutput, CV_GRAY2RGB);
  wxImage test(rgbOutput.rows, rgbOutput.cols, rgbOutput.data, true);
  wxInitAllImageHandlers();
}

void Wx2CvImage(const wxImage& wxImg, cv::Mat& cvImg)
{
    int w = wxImg.GetWidth();
    int h = wxImg.GetHeight();
    int mapping[] = {0,2,1,1,2,0};
    Mat cvRGBImg(h, w, CV_8UC3, wxImg.GetData());
    cvImg = Mat(h, w, CV_8UC3);
    mixChannels(&cvRGBImg, 1, &cvImg, 1, mapping, 3);

}

void Wx2CvImage(wxImage& wxImg, cv::Mat& cvImg, bool destroySrcImg)
{
  Wx2CvImage(wxImg, cvImg);

  if (destroySrcImg)
  {
      wxImg.Destroy();
  }
}

cv::Mat convertType(const cv::Mat& srcImg, int toType, double alpha, double beta)
{
  Mat dstImg;
  srcImg.convertTo(dstImg, toType, alpha, beta);
  return dstImg;
}

cv::Mat rgb2gray(const cv::Mat& rgbImg)
{
  Mat grayImg;
  cv::cvtColor(rgbImg, grayImg, CV_RGB2GRAY);
  return grayImg;
}