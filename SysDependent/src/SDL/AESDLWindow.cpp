/*
 * AESDLWindow.cpp
 *
 *  Created on: Sep 17, 2011
 *      Author: klokik
 */

#include <pthread.h>
#include <signal.h>

#include "SDL/AESDLWindow.h"
#include "AEEngine.h"

using namespace aengine;

void *RunWndProc(void* param)
{
	return ((AESDLWindow*)param)->WndProc(param);
}

AESDLWindow::AESDLWindow(void *engine)
{
	if(engine)
	{
		this->i_engine=(void*)engine;
		this->call_engine_events=true;
	}

	this->vidFlags=0;
	this->vidInfo=NULL;

	this->width=0;
	this->height=0;
	this->bpp=0;

	this->screen=NULL;

	this->active=false;
	this->done=false;
	this->_time=0;

	this->OnClose	=	NULL;
	this->OnKeyDown	=	NULL;
	this->OnKeyUp	=	NULL;
	this->OnMouseDown=	NULL;
	this->OnMouseMove=	NULL;
	this->OnMouseUp	=	NULL;
	this->OnResize	=	NULL;
	this->OnStart	=	NULL;

	this->callback = NULL;

	this->thread=0;
}

void *AESDLWindow::WndProc(void* wparam)
{
	//screen=SDL_SetVideoMode(width,height,bpp,vidFlags);

	if(!screen)
	{
		fprintf(stderr,"Setting Video Mode %dx%d %d [FAILED]: %s;\n",width,height,bpp,SDL_GetError());
		return NULL;
	}
	else
		fprintf(stdout,"Setting Video Mode %dx%d %d [OK]\n",width,height,bpp);

	this->CallEvent(AE_EVENT_START,NULL);

	unsigned int a_event=0;
	int param[5];
	while(!this->done)//&&SDL_WaitEvent(&event))	//PollEvent/WaitEvent
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_ACTIVEEVENT:
				this->active=event.active.gain;
				break;

			case SDL_VIDEORESIZE:
				this->width=event.resize.w;
				this->height=event.resize.h;
				this->screen=SDL_SetVideoMode(width,height,bpp,vidFlags);

				if(!screen)
				{
					fprintf(stderr,"Setting Video Mode %dx%d %d [FAILED]: %s;\n",width,height,bpp,SDL_GetError());
					return NULL;
				}
				else
					fprintf(stdout,"Setting Video Mode %dx%d %d [OK]\n",width,height,bpp);

				param[0]=this->width;
				param[1]=this->height;

				a_event=AE_EVENT_RESIZE;
				break;

			case SDL_KEYDOWN:
				param[0]=event.key.keysym.sym;

				a_event=AE_EVENT_KEYDOWN;
				break;

			case SDL_KEYUP:
				param[0]=event.key.keysym.sym;

				a_event=AE_EVENT_KEYUP;
				break;

			case SDL_MOUSEMOTION:
				param[0]=event.motion.x;
				param[1]=event.motion.y;
				param[2]=param[0]-(this->width>>1);
				param[3]=param[1]-(this->height>>1);
				param[4]=event.motion.state;

				a_event=AE_EVENT_MOUSEMOVE;
				break;

			case SDL_MOUSEBUTTONDOWN:
				param[0]=event.button.x;
				param[1]=event.button.y;
				param[2]=event.button.button;

				a_event=AE_EVENT_MOUSEDOWN;
				break;

			case SDL_MOUSEBUTTONUP:
				param[0]=event.button.x;
				param[1]=event.button.y;
				param[2]=event.button.button;

				a_event=AE_EVENT_MOUSEUP;
				break;

			case SDL_QUIT:
				this->done=true;

				a_event=AE_EVENT_CLOSE;
				break;

			default:
				a_event=0;
				break;
			}
			this->CallEvent(a_event,param);
		}

		this->CallEvent(AE_REFRESH,param);
	}

	SDL_FreeSurface(screen);

	SDL_Quit();

	return NULL;
}

int AESDLWindow::InitWindow(uint16_t _width, uint16_t _height, uint8_t _bpp, AE_WINDOW_TYPE _type)
{
	width=_width;
	height=_height;
	bpp=_bpp;

	if(SDL_Init(SDL_INIT_VIDEO)<0)
	{
		fprintf(stderr,"Video initialization [FAILED]: %s;\n",SDL_GetError());
		return AE_ERR;
	}
	else
	{
		fprintf(stdout,"Video initialization [OK];\n");
	}

	vidInfo=SDL_GetVideoInfo();

	if(!vidInfo)
		return AE_ERR;

	vidFlags =SDL_HWPALETTE;
	vidFlags|=SDL_RESIZABLE;

	if(_type==AE_SDLGL_WINDOW)
	{
		vidFlags|=SDL_OPENGL;
		vidFlags|=SDL_GL_DOUBLEBUFFER;
	}

	if(vidInfo->hw_available)
		vidFlags|=SDL_HWSURFACE;
	else
		vidFlags|=SDL_SWSURFACE;

	if(vidInfo->blit_hw)
		vidFlags|=SDL_HWACCEL;

	if(_type==AE_SDLGL_WINDOW)
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

	screen=SDL_SetVideoMode(width,height,bpp,vidFlags);

	return AE_OK;
}

void AESDLWindow::Run(AE_CALLBACK _callback,bool async)
{
	this->callback=_callback;
	this->_time=SDL_GetTicks();

	if(async)
		pthread_create(&thread,NULL,RunWndProc,(void*)this);
	else
		this->WndProc(NULL);
}

int AESDLWindow::Join(void)
{
	return pthread_join(this->thread,NULL);
}

void AESDLWindow::Stop(void)
{
	done=true;
	//pthread_kill(this->thread,0);
}

void AESDLWindow::SetCursorPos(uint16_t x,uint16_t y)
{
	SDL_WarpMouse(x,y);
}

void AESDLWindow::CenterCursor(void)
{
	SDL_WarpMouse(this->width>>1,this->height>>1);
}

void AESDLWindow::SwapBuffers(void)
{
	SDL_GL_SwapBuffers();
}

void AESDLWindow::CallEvent(unsigned int a_event,int param[5])
{
	if(!call_engine_events)
	{
		switch(a_event)
		{
		case AE_EVENT_CLOSE:		if(OnClose)	OnClose(param);			break;
		case AE_EVENT_KEYDOWN:		if(OnKeyDown) OnKeyDown(param);		break;
		case AE_EVENT_KEYUP:		if(OnKeyUp)	OnKeyUp(param);			break;
		case AE_EVENT_MOUSEDOWN:	if(OnMouseDown) OnMouseDown(param);	break;
		case AE_EVENT_MOUSEMOVE:	if(OnMouseMove) OnMouseMove(param);	break;
		case AE_EVENT_MOUSEUP:		if(OnMouseUp) OnMouseUp(param);		break;
		case AE_EVENT_RESIZE:		if(OnResize) OnResize(param);		break;
		case AE_EVENT_START:		if(OnStart)	OnStart(param);			break;
		case AE_REFRESH:			if(callback) callback(NULL);		break;
		}
	}
	else
	{
		AEEngine *_engine=(AEEngine*)i_engine;
		if(_engine)
		{
			switch(a_event)
			{
			case AE_EVENT_CLOSE:	_engine->i_OnClose(param);			break;
			case AE_EVENT_KEYDOWN:	_engine->i_OnKeyDown(param);		break;
			case AE_EVENT_KEYUP:	_engine->i_OnKeyUp(param);			break;
			case AE_EVENT_MOUSEDOWN:_engine->i_OnMouseDown(param);		break;
			case AE_EVENT_MOUSEMOVE:_engine->i_OnMouseMove(param);		break;
			case AE_EVENT_MOUSEUP:	_engine->i_OnMouseUp(param);		break;
			case AE_EVENT_RESIZE:	_engine->i_OnResize(param);			break;
			case AE_EVENT_START:	_engine->i_OnStart(param);			break;
			case AE_REFRESH:
				uint32_t tmp=SDL_GetTicks();
				param[0]=tmp-this->_time;
				this->_time=tmp;

				_engine->i_Refresh(param);
				break;
			}
		}
	}
}

AESDLWindow::~AESDLWindow(void)
{
	// For some reason this often causes error
	// SDL_FreeSurface(this->screen);
	SDL_Quit();
}

