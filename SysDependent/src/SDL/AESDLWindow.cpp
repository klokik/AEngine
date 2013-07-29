/*
 * AESDLWindow.cpp
 *
 *  Created on: Sep 17, 2011
 *      Author: klokik
 */

#include <signal.h>

#include "SDL/AESDLWindow.h"
#include "AEEngine.h"

namespace aengine
{
	AESDLWindow::AESDLWindow(void *engine)
		:AEWindow(engine)
	{
		this->vidFlags=0;
		this->vidInfo=nullptr;

		this->screen=nullptr;
	}

	void *AESDLWindow::WndProc(void* wparam)
	{
		//screen=SDL_SetVideoMode(width,height,bpp,vidFlags);

		if(!screen)
		{
			fprintf(stderr,"Setting Video Mode %dx%d %d [FAILED]: %s;\n",width,height,bpp,SDL_GetError());
			return nullptr;
		}
		else
			fprintf(stdout,"Setting Video Mode %dx%d %d [OK]\n",width,height,bpp);

		this->CallEvent(AE_EVENT_START,nullptr);

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
						return nullptr;
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
					param[1]=this->height-event.motion.y;
					param[2]=event.motion.xrel;
					param[3]=-event.motion.yrel;
					param[4]=event.motion.state;

					a_event=AE_EVENT_MOUSEMOVE;
					break;

				case SDL_MOUSEBUTTONDOWN:
					param[0]=event.button.x;
					param[1]=this->height-event.button.y;
					param[2]=event.button.button;

					a_event=AE_EVENT_MOUSEDOWN;
					break;

				case SDL_MOUSEBUTTONUP:
					param[0]=event.button.x;
					param[1]=this->height-event.button.y;
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

		return nullptr;
	}

	int AESDLWindow::InitWindow(uint16_t _width, uint16_t _height, uint8_t _bpp,AE_WINDOW_TYPE _type)
	{
		AEWindow::InitWindow(_width,_height,_bpp,_type);

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
		{
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, bpp);
		}

		screen=SDL_SetVideoMode(width,height,bpp,vidFlags);

		return AE_OK;
	}

	void AESDLWindow::SetCursorPos(uint16_t x,uint16_t y)
	{
		SDL_WarpMouse(x,y);
	}

	void AESDLWindow::CenterCursor(void)
	{
		SDL_WarpMouse(this->width/2,this->height/2);
	}

	void AESDLWindow::SwapBuffers(void)
	{
		SDL_GL_SwapBuffers();
	}

	AESDLWindow::~AESDLWindow(void)
	{
		// For some reason this often causes error
		// SDL_FreeSurface(this->screen);
		SDL_Quit();
	}
}
