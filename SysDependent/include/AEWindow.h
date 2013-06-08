///*
// * AEWindow.h
// *
// *  Created on: Oct 13, 2012
// *      Author: klokik
// */
//
#include "SDL/AESDLWindow.h"
//#ifndef AEWINDOW_H_
//#define AEWINDOW_H_
//
//#include <stddef.h>
//
//#include "AEDefines.h"
//
//class AEWindow
//{
//friend void *RunWndProc(void* param);
//
//protected:
//	bool done;
//	bool active;
//
//	void *i_engine;
//	bool call_engine_events;
//
//	AE_CALLBACK callback;
//
//	unsigned long int thread;
//
//	uint32_t _time;
//
//	void *WndProc(void *param);
//
//	void CallEvent(unsigned int a_event,int param[5]);
//
//public:
//
//	//FIXME Use delegates, dude
//	AE_EVENT OnClose;			//attributes {NULL}
//	AE_EVENT OnKeyDown;			//attributes {int keycode}
//	AE_EVENT OnKeyUp;			//attributes {int keycode}
//	AE_EVENT OnMouseDown;		//attributes {int x,y,mousekey}
//	AE_EVENT OnMouseMove;		//attributes {int x,y,deltaX,deltaY,keys}
//	AE_EVENT OnMouseUp;			//attributes {int x,y,mousekey}
//	AE_EVENT OnResize;			//attributes {int width,height}
//	AE_EVENT OnStart;			//attributes {NULL}
//
//	AEWindow(void *engine=NULL);
//
//	int InitWindow(uint16_t _width,uint16_t _height,uint8_t _bpp,AE_WINDOW_TYPE _type);
//
//	void Run(AE_CALLBACK _callback,bool async=false);
//	int Join(void);
//	void Stop(void);
//
//	void SetCursorPos(uint16_t x,uint16_t y);
//	void CenterCursor(void);
//
//	void SwapBuffers(void);
//};
//
//#endif /* AEWINDOW_H_ */
