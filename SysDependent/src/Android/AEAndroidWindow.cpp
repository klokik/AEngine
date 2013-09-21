/*
 * AESDLWindow.h
 *
 *  Created on: Jul 29, 2013
 *      Author: klokik
 */

#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "AEngine", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "AEngine", __VA_ARGS__))

#include "AEResourceManager.h"

#include "Android/AEAndroidWindow.h"

//future definition for app entry point
int main(int argc,char **argv);

extern "C"
{
	static struct android_app *start_state=nullptr;
	// Android entry point
	void android_main(struct android_app *state)
	{
		// check if glue was not stripped
		app_dummy();

		LOGI("App started");

		start_state=state;

		aengine::AEResourceManager::SetWorkDirectory("/sdcard/aengine/");

		main(1,(char**)&state);
	}
}

namespace aengine
{
	AEAndroidWindow::AEAndroidWindow(void *engine)
		:AEWindow(engine)
	{
		display=EGL_NO_DISPLAY;
		context=EGL_NO_CONTEXT;
		surface=EGL_NO_SURFACE;

		a_app=start_state;//nullptr;

		this->first_init_call=true;
		this->break_after_init=false;

		LOGI("Window created");
	}

	void *AEAndroidWindow::WndProc(void* wparam)
	{
		LOGW("Wnd proc started");

		struct android_app *state=this->a_app;

		state->onAppCmd=AEAndroidWindow::HandleCmd;
		state->onInputEvent=AEAndroidWindow::HandleInput;
		state->userData=this;
		LOGI("Window: %d",this);

		if(state->savedState)
		{
			// We are starting with a previous saved state; restore from it.
			//engine.state = *(struct saved_state*)state->savedState;
			LOGI("Attempt to restore state");
		}
		else
			LOGI("Nothing to restore");

		this->CallEvent(AE_EVENT_START,nullptr);

		// loop waiting for stuff to do.

		int param[5]={0};
		unsigned int a_event=0;
		while(!done)
		{
			// Read all pending events.
			int ident;
			int events;
			struct android_poll_source* source;

			// If not active, we will block forever waiting for events.
			// If active, we loop until all events are read, then continue
			// to draw the next frame of animation.
			while((ident=ALooper_pollAll(this->active?0:-1,NULL,&events,
					reinterpret_cast<void**>(&source)))>=0)
			{
				// Process this event.
				if(source)
				{
					source->process(state,source);
				}

				// If a sensor has data, process it now.
				if(ident==LOOPER_ID_USER)
				{
					//nop
				}

				// Check if we are exiting.
				if(state->destroyRequested!=0)
				{
					a_event=AE_EVENT_CLOSE;
					done=true;
					LOGI("Destroy requested");
				}

				if(a_event)
				{
					this->CallEvent(a_event,param);
					a_event=0;
				}
			}

			if(this->break_after_init)
			{
				this->break_after_init=false;
				break;
			}

			if(active)
			{
				this->CallEvent(AE_REFRESH,param);
			}
		}

		return nullptr;
	}

	int32_t AEAndroidWindow::HandleInput(struct android_app *app,AInputEvent *event)
	{
		AEAndroidWindow *wnd=static_cast<AEAndroidWindow*>(app->userData);

		if(AInputEvent_getType(event)==AINPUT_EVENT_TYPE_MOTION)
		{
			float x=AMotionEvent_getX(event,0);
			float y=AMotionEvent_getY(event,0);

			static float last_x=x;
			static float last_y=y;

			int param[]={
				(int)x,
				(int)(wnd->height-y),
				(int)(x-last_x),
				-(int)(y-last_y),
				2
			};
			last_x=x;
			last_y=y;

			switch(AMotionEvent_getAction(event))
			{
			case AMOTION_EVENT_ACTION_DOWN:
				LOGW("Action DOWN");
				param[2]=2;
				param[3]=0;
				wnd->CallEvent(AE_EVENT_MOUSEDOWN,param);
				break;

			case AMOTION_EVENT_ACTION_UP:
				LOGW("Action UP");
				param[2]=2;
				param[3]=0;
				wnd->CallEvent(AE_EVENT_MOUSEUP,param);
				break;

			case AMOTION_EVENT_ACTION_MOVE:
				LOGW("Action MOVE");
				wnd->CallEvent(AE_EVENT_MOUSEMOVE,param);
				break;
			}
		}
		else if(AInputEvent_getType(event)==AINPUT_EVENT_TYPE_KEY)
		{
			LOGW("Key input");
		}
		return 0;
	}

	void AEAndroidWindow::HandleCmd(struct android_app *app,int32_t cmd)
	{
		AEAndroidWindow *wnd=static_cast<AEAndroidWindow*>(app->userData);

		int param[5]={0};
		switch(cmd)
		{
		case APP_CMD_SAVE_STATE:
			// The system has asked us to save our current state.  Do so.
			// wnd->app->savedState = malloc(sizeof(struct saved_state));
			// *((struct saved_state*)wnd->app->savedState) = wnd->state;
			// wnd->app->savedStateSize = sizeof(struct saved_state);
			// wnd->engine->Suspend()
			LOGI("cmd: Save state");
			break;
		case APP_CMD_INIT_WINDOW:
			// The window is being shown, get it ready.
			if(wnd->a_app->window&&wnd->context==EGL_NO_CONTEXT)
			{
				LOGI("cmd: Init window");
				if(!wnd->InitDisplay())
				{
					LOGW("Display init FAILED");
				}
				if(wnd->first_init_call)
				{
					wnd->first_init_call=false;
					wnd->break_after_init=true;
				}
			}
			break;
		case APP_CMD_TERM_WINDOW:
			// The window is being hidden or closed, clean it up.
			wnd->TerminateDisplay();
			LOGI("cmd: Terminate window");
			break;
		case APP_CMD_GAINED_FOCUS:
			wnd->active=true;
			LOGI("cmd: Window %d activated",wnd);
			break;
		case APP_CMD_LOST_FOCUS:
			wnd->active=false;
			LOGI("cmd: Window lost focus");
			break;
		case APP_CMD_WINDOW_RESIZED:
			break;
		case APP_CMD_CONFIG_CHANGED:
			break;
		}

		if(wnd->active)
		{
			int32_t new_width=ANativeWindow_getWidth(wnd->a_app->window);
			int32_t new_height=ANativeWindow_getHeight(wnd->a_app->window);
			if(wnd->width!=new_width||wnd->height!=new_height)
			{
				LOGI("cmd: Window resized");
				param[0]=wnd->width=new_width;
				param[1]=wnd->height=new_height;
				wnd->CallEvent(AE_EVENT_RESIZE,param);
			}
		}
	}

	int AEAndroidWindow::InitDisplay(void)
	{
		if(!a_app)
		{
			LOGW("Something's wrong with world::a_app");
			return AE_ERR;
		}

		// initialize EGL

		EGLint w,h,format;
		EGLSurface surface;
		EGLContext context;

		EGLDisplay display=eglGetDisplay(EGL_DEFAULT_DISPLAY);

		EGLint major_version;
		EGLint minor_version;

		eglInitialize(display,&major_version,&minor_version);
		LOGI("EGL %i.%i",major_version,minor_version);

		//query num of configs
		int num_conf=0;
		eglGetConfigs(display,nullptr,0,&num_conf);  //if configuration array is null it still returns the number of configurations

		LOGI("total num configs: %i",num_conf);

		//now query the configs
		EGLConfig* conf=new EGLConfig[num_conf];
		eglGetConfigs(display,conf,num_conf,&num_conf);

		int r=0;
		int g=0;
		int b=0;
		int a=0;
		int depth=0;
		int stencil=0;
		int render_type=0;
		int surface_type=0;
		int format_type=0;

		//this is the one true config that we will use
		EGLConfig config_to_use;

		for(int q=0;q<num_conf;q++)
		{
			eglGetConfigAttrib(display,conf[q],EGL_RED_SIZE,&r);
			eglGetConfigAttrib(display,conf[q],EGL_GREEN_SIZE,&g);
			eglGetConfigAttrib(display,conf[q],EGL_BLUE_SIZE,&b);
			eglGetConfigAttrib(display,conf[q],EGL_ALPHA_SIZE,&a);
			eglGetConfigAttrib(display,conf[q],EGL_DEPTH_SIZE,&depth);
			eglGetConfigAttrib(display,conf[q],EGL_STENCIL_SIZE,&stencil);
			eglGetConfigAttrib(display,conf[q],EGL_RENDERABLE_TYPE,&render_type);
			eglGetConfigAttrib(display,conf[q],EGL_SURFACE_TYPE,&surface_type);
			eglGetConfigAttrib(display,conf[q],EGL_NATIVE_VISUAL_ID,&format_type);

			LOGI("%d: (R%dG%dB%dA%d)depth:(%d) stencil:(%d) surfaceType:(%d) renderType:(%d) formatType:(%d)",
				q, r,g,b,a,depth,stencil,surface_type, render_type,format_type);

			if((render_type&EGL_OPENGL_ES2_BIT)&&
				(surface_type&EGL_WINDOW_BIT)&&
				(format_type&WINDOW_FORMAT_RGB_565)&&
				depth>0)
			{
				config_to_use=conf[q];

				LOGI("Using config #%d",q);
				break;
			}
		}

		//bridge the pixel format back into android
		eglGetConfigAttrib(display,config_to_use,EGL_NATIVE_VISUAL_ID,&format);
		ANativeWindow_setBuffersGeometry(a_app->window,0,0,format);

		surface=eglCreateWindowSurface(display,config_to_use,a_app->window,nullptr);

		if(surface==EGL_NO_SURFACE)
		{
			LOGW("Error making surface, EGL_NO_SURFACE");
			return AE_ERR;
		}

		//now create the OpenGL ES2 context
		const EGLint contextAttribs[]=
		{
				EGL_CONTEXT_CLIENT_VERSION, 2,
				EGL_NONE
		};

		context=eglCreateContext(display,config_to_use,EGL_NO_CONTEXT,contextAttribs);

		if(context==EGL_NO_CONTEXT)
		{
			LOGW("Error making context, EGL_NO_CONTEXT");
			return AE_ERR;
		}

		if(eglMakeCurrent(display,surface,surface,context)==EGL_FALSE)
		{
			LOGW("Unable to eglMakeCurrent");
			return AE_ERR;
		}

		eglQuerySurface(display,surface,EGL_WIDTH,&w);
		eglQuerySurface(display,surface,EGL_HEIGHT,&h);

		this->display=display;
		this->context=context;
		this->surface=surface;
		this->width=w;
		this->height=h;

		LOGI("Renderer: %s",glGetString(GL_VERSION));

		if(width!=w||height!=h)
		{
			width=w;
			height=h;

			int param[5];
			param[0]=this->width;
			param[1]=this->height;
			this->CallEvent(AE_EVENT_RESIZE,param);
		}
		
		LOGI("Display init succeed: %dx%d",w,h);

		return AE_OK;
	}

	void AEAndroidWindow::TerminateDisplay(void)
	{
		if(display!=EGL_NO_DISPLAY)
		{
			eglMakeCurrent(display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);
			if(context!=EGL_NO_CONTEXT)
			{
				eglDestroyContext(display,context);
			}
			if(surface!=EGL_NO_SURFACE)
			{
				eglDestroySurface(display,surface);
			}
			eglTerminate(display);
		}
		display=EGL_NO_DISPLAY;
		context=EGL_NO_CONTEXT;
		surface=EGL_NO_SURFACE;

		this->active=false;
	}

	int AEAndroidWindow::InitWindow(uint16_t _width,uint16_t _height,uint8_t _bpp,AE_WINDOW_TYPE _type)
	{
		LOGI("Init android window and wait for it to be shown");

		AEWindow::InitWindow(_width,_height,_bpp,_type);

		this->break_after_init=false;
		this->WndProc(nullptr);

		return AE_OK;
	}

	void AEAndroidWindow::SetCursorPos(uint16_t x,uint16_t y)
	{
		LOGW("Mouse pointer is currently unsupported on target platform");
	}

	void AEAndroidWindow::CenterCursor(void)
	{
		LOGW("Mouse pointer is currently unsupported on target platform");
	}

	void AEAndroidWindow::SwapBuffers(void)
	{
		eglSwapBuffers(display,surface);
	}

	AEAndroidWindow::~AEAndroidWindow(void)
	{
		TerminateDisplay();
		LOGI("Window destroyed");
	}
}