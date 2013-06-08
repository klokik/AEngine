/*
 * AESDLWindow.h
 *
 *  Created on: Sep 17, 2011
 *      Author: klokik
 */

#ifndef AESDLWINDOW_H_
#define AESDLWINDOW_H_

#include <SDL/SDL.h>

#include "AEDefines.h"
//#include "AEEngine.h"


//TODO make window separate abstract class to make possible use different window systems
class AESDLWindow
{
	friend void *RunWndProc(void* param);

protected:
	uint16_t width;
	uint16_t height;
	uint16_t bpp;

	bool done;
	bool active;

	int vidFlags;
	const SDL_VideoInfo *vidInfo;
	SDL_Event event;
	SDL_Surface *screen;

	void *i_engine;
	bool call_engine_events;

	AE_CALLBACK callback;

	unsigned long int thread;

	uint32_t _time;

	void *WndProc(void *param);

	void CallEvent(unsigned int a_event,int param[5]);

public:
	//try to use engine events instead of main events
	//used in engine class

	AE_EVENT OnClose;			//attributes {NULL}
	AE_EVENT OnKeyDown;			//attributes {int keycode}
	AE_EVENT OnKeyUp;			//attributes {int keycode}
	AE_EVENT OnMouseDown;		//attributes {int x,y,mousekey}
	AE_EVENT OnMouseMove;		//attributes {int x,y,deltaX,deltaY,keys}
	AE_EVENT OnMouseUp;			//attributes {int x,y,mousekey}
	AE_EVENT OnResize;			//attributes {int width,height}
	AE_EVENT OnStart;			//attributes {NULL}

	AESDLWindow(void *engine=NULL);

	int InitWindow(uint16_t _width,uint16_t _height,uint8_t _bpp,AE_WINDOW_TYPE _type);

	void Run(AE_CALLBACK _callback,bool async=false);
	int Join(void);
	void Stop(void);

	void SetCursorPos(uint16_t x,uint16_t y);
	void CenterCursor(void);

	void SwapBuffers(void);

	virtual ~AESDLWindow(void);
};

#endif /* AESDLWINDOW_H_ */
