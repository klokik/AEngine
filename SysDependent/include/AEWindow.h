/*
* AEWindow.h
*
*  Created on: Jul 29, 2013
*      Author: klokik
*/

#ifndef AEWINDOW_H_
#define AEWINDOW_H_

#include <stddef.h>
#include <pthread.h>
#include <chrono>

#include "AEDefines.h"


namespace aengine
{
	class AEWindow
	{
		friend void *RunWndProc(void* param);

	protected:
		uint16_t width;
		uint16_t height;
		uint16_t bpp;

		bool done;
		bool active;

		void *i_engine;
		bool call_engine_events;

		AE_CALLBACK callback;

		bool thread_running;
		pthread_t thread;

		std::chrono::system_clock::time_point last_tp;

		//contains main app loop that receive events
		virtual void *WndProc(void *param)=0;

		void CallEvent(unsigned int a_event,int param[5]);

	public:

		//FIXME Use delegates, dude
		AE_EVENT OnClose;			//attributes {nullptr}
		AE_EVENT OnKeyDown;			//attributes {int keycode}
		AE_EVENT OnKeyUp;			//attributes {int keycode}
		AE_EVENT OnMouseDown;		//attributes {int x,y,mousekey}
		AE_EVENT OnMouseMove;		//attributes {int x,y,deltaX,deltaY,keys}
		AE_EVENT OnMouseUp;			//attributes {int x,y,mousekey}
		AE_EVENT OnResize;			//attributes {int width,height}
		AE_EVENT OnStart;			//attributes {nullptr}

		AEWindow(void *engine=nullptr);

		//store coresponding values to class 
		virtual int InitWindow(uint16_t _width,uint16_t _height,uint8_t _bpp,AE_WINDOW_TYPE _type);

		void Run(AE_CALLBACK _callback,bool async=false);
		int Join(void);
		void Stop(void);

		virtual void SetCursorPos(uint16_t x,uint16_t y)=0;
		virtual void CenterCursor(void)=0;

		virtual void SwapBuffers(void)=0;

		virtual ~AEWindow(void);
	};


	AEWindow *GetPlatformWindow(void *engine=nullptr);
}

#endif /* AEWINDOW_H_ */
