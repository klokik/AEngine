/*
* AE computer vision library
* Author: klokik
*/

#ifndef AEVISION_H_
#define AEVISION_H_

#include <string>
#include <vector>
#include <map>
#include <queue>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/background_segm.hpp>

namespace aengine
{
	class AETrackingObject
	{
	public:
		// position
		// to be replaced by vector components
		float X;
		float Y;
		float Z;	//currently unused but may be used in future if decide to track objects in 3d space somehow

		std::string name;
		bool active;
		bool visible;

		cv::MatND histogram;
		std::vector<cv::Point> contour;

		AETrackingObject(std::string name);
		~AETrackingObject(void);
	};

	class AEObjectTracker
	{
	protected:
		pthread_t loop_thread;
		bool done;
		int camera;

		cv::VideoCapture capture;

		cv::Mat source;
		cv::Mat output;
		cv::Mat fg_mask;	

		std::map<std::string,AETrackingObject> tracking_objects;
		std::queue<std::string> requested_objects;

		cv::Mat background;
		cv::BackgroundSubtractorMOG2 bg_model;

		void *main_loop(void *args);
		static void *thread_runner(void *args);

		void prepare_image(void);
		void find_active_objects(void);
		void draw_ui(void);
		void draw_histograms(void);
		void get_histogram(AETrackingObject &object);
		void handle_input(void);
		void fetch_object(void);
	public:
		float fetch_size;
		float blur_size;
		float threshold_value;
		int histo_size;
		bool update_bg_model;

		cv::Size process_size;

		std::string message;

		AEObjectTracker(int camera);

		void Run(bool separate_thread);
		void Stop(void);
		void RequestObject(std::string name);
		void DropObject(std::string name);
		bool IsReady(std::string name);
		bool IsVisible(std::string name);
		void SetActive(std::string name,bool active);
		void DrawTrackingObjects(void);
		void DrawObject(std::string name,int color);

		void FetchBackground(void);
		void SubtractBackground(void);

		void CalibrateCamera(void);
		void ChangeCamera(int camera);

		const AETrackingObject &operator [] (std::string name);


		~AEObjectTracker(void);
	};
}

#endif /* AEVISION_H_ */