/*
* AEWindow.cpp
*
*  Created on: Jul 29, 2013
*      Author: klokik
*/

#include "AEWindow.h"
#include "AEEngine.h"

namespace aengine
{
	void *RunWndProc(void* param)
	{
		AEWindow *wnd=static_cast<AEWindow*>(param);

		wnd->thread_running=true;
		void *result=wnd->WndProc(nullptr);
		wnd->thread_running=false;
		return result;
	}

	AEWindow::AEWindow(void *engine)
	{
		//XXX i don't like it
		if(engine)
		{
			this->i_engine=(void*)engine;
			this->call_engine_events=true;
		}

		this->width=0;
		this->height=0;
		this->bpp=0;

		this->active=false;
		this->done=false;

		this->OnClose	=	nullptr;
		this->OnKeyDown	=	nullptr;
		this->OnKeyUp	=	nullptr;
		this->OnMouseDown=	nullptr;
		this->OnMouseMove=	nullptr;
		this->OnMouseUp	=	nullptr;
		this->OnResize	=	nullptr;
		this->OnStart	=	nullptr;

		this->callback = nullptr;

		this->thread_running=false;
	}

	void AEWindow::CallEvent(unsigned int a_event,int param[5])
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
			if(i_engine)
			{
				AEEngine *_engine=static_cast<AEEngine*>(i_engine);

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
					auto now=std::chrono::system_clock::now();
					param[0]=std::chrono::duration_cast<std::chrono::milliseconds>
						(now-last_tp).count();
					this->last_tp=now;

					_engine->i_Refresh(param);
					break;
				}
			}
		}
	}

	int AEWindow::InitWindow(uint16_t _width, uint16_t _height, uint8_t _bpp,AE_WINDOW_TYPE _type)
	{
		width=_width;
		height=_height;
		bpp=_bpp;
		
		return AE_OK;
	}

	void AEWindow::Run(AE_CALLBACK _callback,bool async)
	{
		this->callback=_callback;
		this->last_tp=std::chrono::system_clock::now();

		if(async)
			pthread_create(&thread,nullptr,RunWndProc,static_cast<void*>(this));
		else
			this->WndProc(nullptr);
	}

	void AEWindow::Stop(void)
	{
		done=true;
		if(thread_running)
			Join();
	}

	int AEWindow::Join(void)
	{
		return pthread_join(this->thread,nullptr);
	}

	AEWindow::~AEWindow(void)
	{
		if(!done)
			Stop();
	}
}