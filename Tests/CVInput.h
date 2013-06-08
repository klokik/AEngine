#include <iostream>
#include <stdio.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <pthread.h>

#include "AEVectorMath.h"

using namespace cv;
using namespace std;

void draw_histos(void);
void build_histogram(Mat &src,Rect rect);
void get_blobs(Mat &src);
void *CVCoord_cycle(void *arg);

// Quantize the hue to 30 levels
// and the saturation to 32 levels
int hbins = 16, sbins = 16;
int histsize[] = {hbins, sbins};
// hue varies from 0 to 179, see cvtcolor
float hranges[] = { 0, 180 };
// saturation varies from 0 (black-gray-white) to
// 255 (pure spectrum color)
float sranges[] = { 0, 256 };
const float* ranges[] = { hranges, sranges };
// we compute the histogram from the 0-th and 1-st channels
int channels[] = {0, 1};
Mat cam_src;
Rect capture_rect(320,200,40,40);

std::vector<MatND> histos;

AEVector3f track_position=vec3f(0,0,0);
float track_length=0;

void CoordRun(void)
{
	pthread_t thread;
	pthread_create(&thread,NULL,CVCoord_cycle,NULL);
}

void *CVCoord_cycle(void *arg)
{
  VideoCapture capture;
  capture.open(0);
 
  namedWindow( "Source", 1 );
  namedWindow( "H-S Histogram", 1 );

  bool done=false;

  MatND hist;

  while(!done)
  {
    Mat hsv;

    capture.read(cam_src);
    flip(cam_src,cam_src,1);
    // threshold(cam_src,cam_src,50,255,THRESH_TOZERO);
    // medianBlur(src.clone(),src,13);
    cvtColor(cam_src, hsv, CV_BGR2HSV);


    // Canny(src,src,0,20*3,3);

    get_blobs(hsv);

    rectangle(cam_src, capture_rect,Scalar(255,255,255),2);

    imshow( "Source", cam_src );

    switch(waitKey(30))
    {
      case 27:
        done = true;
        break;

      case 32:
        build_histogram(hsv,capture_rect);
        draw_histos();        
        break;
    }
  }

  cvDestroyWindow("Source");
  cvDestroyWindow("H-S Histogram");
}

void draw_histos(void)
{
  if(histos.size()==0)
    return;

  int scale = 10;
  Mat histimg = Mat((sbins+1)*scale*histos.size(), hbins*scale, CV_8UC3,Scalar(0,0,255));

  for(size_t q=0;q<histos.size();q++)
  {
    const MatND &hist=histos[q];

    double maxval=0;
    minMaxLoc(hist, 0, &maxval, 0, 0);

    for( int h = 0; h < hbins; h++ )
      for( int s = 0; s < sbins; s++ )
      {
          float binval = hist.at<float>(h, s);
          int intensity = cvRound(binval*255/maxval);
          rectangle( histimg, Point(h*scale, (s+q*(sbins+1))*scale),
                      Point( (h+1)*scale - 1, (s+1+q*(sbins+1))*scale - 1),
                      Scalar(h*180/hbins,s*255/sbins,intensity),
                      CV_FILLED ); 
      }

  }  

  cvtColor(histimg,histimg,CV_HSV2BGR);
  imshow( "H-S Histogram", histimg );
}

void build_histogram(Mat &src,Rect rect)
{
  MatND hist;

  Mat sub_img = src(rect);
  calcHist( &sub_img, 1, channels, Mat(), // do not use mask
           hist, 2, histsize, ranges,
           true, // the histogram is uniform
           false );

  normalize(hist,hist,0,255, NORM_MINMAX,-1, Mat());

  histos.push_back(hist);
}

void get_blobs(Mat &src)
{
  Mat blured;
  resize(src,blured,Size(320,240));
  GaussianBlur(blured,blured,Size(17,17),0);
  for(size_t q=0;q<histos.size();q++)
  {
    //First we build back-projection
    Mat blobs;
    calcBackProject(&blured,1,channels,histos[q],blobs,ranges);
    //threshold(blobs,blobs,20,255,THRESH_BINARY);

    // imshow("Source",blobs);
    // break;

    vector<vector<Point> > contours;

    findContours(blobs,contours,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE);

    Scalar a_color(0,0,255);
    Scalar r_color(0,255,0);
    double max_crit = 50;
    int max_id = -1;
    for(size_t w=0;w<contours.size();w++)
    {
      for(size_t e=0;e<contours[w].size();e++) //Scale contours
        contours[w][e]*=2;

      double crit = arcLength(contours[w],true);
      if(crit<50)
        continue;

      if(crit>max_crit)
      {
        max_crit=crit;
        max_id=w;
      }
      drawContours(cam_src,contours,w,a_color,1);
    }

    if(max_id!=-1)
    {
      drawContours(cam_src,contours,max_id,r_color,4);

      Rect prect=boundingRect(contours[max_id]);
      Point pos(prect.x+prect.width/2,prect.y+prect.height/2);

      track_position.X=pos.x-320;
      track_position.Y=-pos.y+240;
      track_length = max_crit;

      char buf[8];
      snprintf(buf,8,"%d",q);

      circle(cam_src,pos,(prect.width+prect.height)/8,Scalar(0,255,255),3);
      putText(cam_src,buf,pos,FONT_HERSHEY_SIMPLEX,1,Scalar(0,255,255),2);
      cout<<"Position ("<<pos.x<<","<<pos.y<<");"<<endl;
    }
  }
}
