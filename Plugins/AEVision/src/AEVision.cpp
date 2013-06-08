#include <pthread.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "AEVision.h"

using namespace cv;
using namespace aengine;

AEObjectTracker::AEObjectTracker(int camera=0):
	bg_model(10000,16,false)
{
	fetch_size = 32;
	blur_size = 17;
	threshold_value = 30;
	histo_size = 16;
	this->camera = camera;
	process_size = Size(320,240);
	update_bg_model = true;
	bg_model.set("nmixtures",3);
	message = "AEVision";
}

void AEObjectTracker::Run(bool separate_thread=true)
{
	done = false;
	if(separate_thread)
	{
		pthread_create(&loop_thread,NULL,thread_runner,this);
	}
	else
		main_loop(NULL);
}

void AEObjectTracker::Stop(void)
{
	done=true;
	pthread_join(loop_thread,NULL);
}

void AEObjectTracker::RequestObject(std::string name)
{
	requested_objects.push(name);
	if(requested_objects.size()==1)
	{
		message = "Requested object: ";
		message.append(requested_objects.front());
	}
}

void AEObjectTracker::DropObject(std::string name)
{
	tracking_objects.erase(name);
}

bool AEObjectTracker::IsReady(std::string name)
{
	return static_cast<bool>(tracking_objects.count(name));
}

bool AEObjectTracker::IsVisible(std::string name)
{
	return tracking_objects.at(name).visible;
}

void AEObjectTracker::SetActive(std::string name,bool active)
{
	tracking_objects.at(name).active=active;
}

void AEObjectTracker::DrawTrackingObjects(void)
{
	for(std::map<string,AETrackingObject>::iterator iter=tracking_objects.begin();iter!=tracking_objects.end();++iter)
	{
		if(iter->second.visible&&iter->second.active)
		{
			DrawObject(iter->first,0);
		}
	}
}

void AEObjectTracker::DrawObject(std::string name,int color)
{
	Scalar s_color=Scalar(255,255,255);

	std::vector< vector<Point> > contours;
	contours.push_back(tracking_objects.at(name).contour);

	drawContours(output,contours,0,s_color,4);

	double scale = 0.5;
	int baseline=0;
	Size text_size=getTextSize(name,FONT_HERSHEY_SIMPLEX,1,scale,&baseline);

	//Point pos(tracking_objects.at(name).X-text_size.width/4,tracking_objects.at(name).Y+text_size.height/2);
	Point pos(
		(tracking_objects.at(name).X+1)*output.cols/2,
		(-tracking_objects.at(name).Y+1)*output.rows/2);
	putText(output,name,pos,FONT_HERSHEY_SIMPLEX,scale,s_color);
}

void AEObjectTracker::FetchBackground(void)
{
	// FUCK IT we don't need it anymore!
	return;

	// we need to get several images
	// during some perion of time and calculate average of them

	int count=10;
	if(!capture.isOpened())
		capture.open(camera);

	if(!capture.isOpened())
		return;

	Mat input;
	capture.read(input);

	Mat bg = Mat(input.rows,input.cols,CV_32FC3,Scalar(0,0,0));

	for(size_t q=0;q<count;q++)
	{
		capture.read(input);
		flip(input, input, 1);	
		input.convertTo(input,CV_32FC3);

		add(bg,input,bg);
	}

	divide(bg,Scalar(count,count,count),bg);

	bg.convertTo(background,CV_16SC3);
}

void AEObjectTracker::SubtractBackground(void)
{
	// update background model
	bg_model(source,fg_mask,update_bg_model?-1:0);
	erode(fg_mask,fg_mask,Mat());
	dilate(fg_mask,fg_mask,Mat());

	// output.convertTo(fg_mask,CV_16SC3);
	// subtract(fg_mask,background,fg_mask);
	// resize(fg_mask,fg_mask,process_size);
	// fg_mask = abs(fg_mask);

	// Mat r(fg_mask.rows,fg_mask.cols,CV_16S);
	// Mat g(r);
	// Mat b(r);
	// Mat out[] = {r,g,b};
	// int from_to[] = { 0,2, 1,1, 2,0 };	//we use BGR alignment
	// mixChannels(&fg_mask,1,out,3,from_to,3);
 
	// add(r,g,r);
	// add(r,b,r);

	// GaussianBlur(r, r, Size(blur_size,blur_size), 0);

	// threshold(r,r,threshold_value,255,THRESH_BINARY);

	// r.convertTo(fg_mask,CV_8U);
}

//void AEObjectTracker::CalibrateCamera(void);

void AEObjectTracker::ChangeCamera(int camera)
{
	this->camera = camera;
	throw 0; //need to modify main loop
}

const AETrackingObject &AEObjectTracker::operator [] (std::string name)
{
	return tracking_objects.at(name);
}

void *AEObjectTracker::main_loop(void *args)
{
	const char *viewport="AEVision viewport";
	//Start camera, create window, etc.
	if(!capture.isOpened())
		capture.open(camera);

	if(!capture.isOpened())
		return NULL;

	namedWindow(viewport,1);

	// set empty background with apropriate size
	capture.read(source);
	background = Mat(source.rows,source.cols,CV_16SC3,Scalar(0,0,0));

	while(!done)
	{
		capture.read(source);
		flip(source, source, 1);
		output=source.clone();



		prepare_image();
		find_active_objects();

		DrawTrackingObjects();

		draw_ui();

		//cvtColor(source,output,CV_HSV2BGR);
		//background.copyTo(output);
		imshow(viewport,output);

		handle_input();
	}

	//Dispose all stuff, close camera, close windows etc.
	destroyWindow(viewport);
}

void *AEObjectTracker::thread_runner(void *args)
{
	reinterpret_cast<AEObjectTracker*>(args)->main_loop(NULL);

	return NULL;
}

void AEObjectTracker::AEObjectTracker::prepare_image(void)
{
	resize(source, source, process_size);

	SubtractBackground();

	GaussianBlur(source, source, Size(blur_size,blur_size),0);

	cvtColor(source, source, CV_BGR2HSV);
}

void AEObjectTracker::find_active_objects(void)
{
	int channels[] = {0,1};
	int size_arr[] = {histo_size,histo_size};
	float hranges[] = {0,180};
	float sranges[] = {0,256};
	const float *ranges[] = {hranges,sranges};

	Mat blobs;
	double min_crit=20;

	for(std::map<string,AETrackingObject>::iterator iter=tracking_objects.begin();iter!=tracking_objects.end();++iter)
	{
		if(!iter->second.active)
			continue;

		calcBackProject(&source,1,channels,iter->second.histogram,blobs,ranges);

		//apply foreground mask
		bitwise_and(blobs,fg_mask,blobs);
		// blobs.copyTo(output);

		std::vector< vector<Point> > contours;

		findContours(blobs,contours,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE);

		size_t max_id=0;
		double max_crit=0;

		for(size_t c=0;c<contours.size();c++)
		{
			double crit = arcLength(contours[c],true);
			if(crit<min_crit)
				continue;

			if(crit>max_crit)
			{
				max_crit=crit;
				max_id=c;
			}
		}

		if(max_crit!=0)
		{
			Point average(0,0);
			iter->second.contour=contours[max_id];
			//Scale contours to darawable size
			for(size_t s=0;s<iter->second.contour.size();s++)
			{
				iter->second.contour[s].x*=output.cols/process_size.width;
				iter->second.contour[s].y*=output.rows/process_size.height;
			}

			iter->second.visible = true;
			Rect prect=boundingRect(iter->second.contour);

			iter->second.X = prect.x+prect.width/2;
			iter->second.Y = prect.y+prect.height/2;
			//scale position to [-1;1] range
			iter->second.X /= output.cols/2;
			iter->second.Y /= output.rows/2;
			iter->second.X -= 1;
			iter->second.Y -= 1;
			//inverse Y component for canonical CS
			iter->second.Y *= -1;

			iter->second.Z = max_crit;	//may be usefull
		}
		else
			iter->second.visible = false;
	}
}

void AEObjectTracker::draw_ui(void)
{
	Mat scaled;

	Rect fetch_rect_scaled(
		(source.cols-fetch_size)/2*output.cols/source.cols,
		(source.rows-fetch_size)/2*output.rows/source.rows,
		fetch_size*output.cols/source.cols,
		fetch_size*output.rows/source.rows);

	resize(fg_mask,fg_mask,output.size());
	multiply(output,Scalar(0.5,0.5,0.5),scaled);
	Mat inv_mask;
	subtract(Scalar(255,255,255),fg_mask,inv_mask);
	scaled.copyTo(output,inv_mask);
	// end draw mask

	rectangle(output, fetch_rect_scaled, Scalar(0,255,255),2);
	draw_histograms();

	putText(
		output,
		message,
		Point(4,output.rows-4),
		FONT_HERSHEY_SIMPLEX,
		1,
		Scalar(0,255,0),
		2);
}

void AEObjectTracker::draw_histograms()
{
	int shift=4;
	float scale=4;

	Mat one = Mat(histo_size*scale,histo_size*scale,CV_8UC3);

	for(std::map<string,AETrackingObject>::iterator iter=tracking_objects.begin();iter!=tracking_objects.end();++iter)
	{
		double maxval=2;
		minMaxLoc(iter->second.histogram, 0, 0);
		for( int h = 0; h < histo_size; h++ )
			for( int s = 0; s < histo_size; s++ )
			{
				float binval = iter->second.histogram.at<float>(h, s);
				int intensity = cvRound(binval*255/maxval);
				rectangle( one, Point(h*scale, s*scale),
					Point( (h+1)*scale-1, (s+1)*scale-1),
					Scalar(h*180/histo_size,s*255/histo_size,intensity),
					CV_FILLED); 
			}

		cvtColor(one,one,CV_HSV2BGR);

		one.copyTo(
			output.colRange(shift,shift+one.cols)
			.rowRange(4,one.rows+4));
		shift+=(histo_size+1)*scale;
	}
}

void AEObjectTracker::get_histogram(AETrackingObject &object)
{
	Rect fetch_rect(
		(source.cols-fetch_size)/2,
		(source.rows-fetch_size)/2,
		fetch_size,
		fetch_size);

	Mat subimage=source(fetch_rect);

	int channels[] = {0,1};
	int size_arr[] = {histo_size,histo_size};
	float hranges[] = {0,180};
	float sranges[] = {0,256};
	const float *ranges[] = {hranges,sranges};

	calcHist(
		&subimage, 1, channels, Mat(),
		object.histogram, 2, size_arr, ranges,
		true, false );

	normalize(object.histogram,object.histogram,0,255,NORM_MINMAX,-1,Mat());
}

void AEObjectTracker::handle_input(void)
{
	switch(waitKey(1))
	{
	case 27:
		//done = true;
		break;

	case 32:
		if(!requested_objects.empty())
			fetch_object();
		if(!requested_objects.empty())
		{
			message = "Requested object: ";
			message.append(requested_objects.front());
		}
		else
			message = "Tracking";
		break;
	case 98:
		FetchBackground();
		break;
	}
}

void AEObjectTracker::fetch_object(void)
{
	// Display message to place object to rectange
	AETrackingObject target(requested_objects.front());

	get_histogram(target);

	target.active = true;

	tracking_objects.insert(std::pair<string,AETrackingObject>(requested_objects.front(),target));

	requested_objects.pop();
}

AEObjectTracker::~AEObjectTracker(void)
{
	//Do some clean up stuff
	// but what!?
}

AETrackingObject::AETrackingObject(std::string name)
{
	this->name = name;
	X = Y = Z = 0;
	active = false;
	visible = false;
}

AETrackingObject::~AETrackingObject(void)
{
	// LOL
}