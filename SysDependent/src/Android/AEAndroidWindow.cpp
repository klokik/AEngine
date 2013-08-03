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

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

#include "AEAndroidWindow.h"

//future definition for app entry point
int main(int argc,char **argv);

extern "C"
{
	// Android entry point
	void android_main(struct android_app *state)
	{
		// check if glue was not stripped
		app_dummy();

		LOGI("App started");

		main(1,(void*)state);
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

		android_app=nullptr;

		LOGI("Window created");
	}

	void *AEAndroidWindow::WndProc(void* wparam)
	{
		if(state->savedState)
		{
			// We are starting with a previous saved state; restore from it.
			//engine.state = *(struct saved_state*)state->savedState;
			LOGI("Attempt to restore state");
		}
		else
			LOGI("Nothing to restore");

		// loop waiting for stuff to do.

		int param[5]={0};
		unsigned int a_event;
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
					static_cast<void**>(&source)))>=0)
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
			}
			this->CallEvent(AE_REFRESH,param);
		}

		return nullptr;
	}

	static int32_t AEAndroidWindow::HandleInput(struct android_app *app,AInputEvent *event)
	{
		if(AInputEvent_getType(event)==AINPUT_EVENT_TYPE_MOTION)
		{
			LOGI("Motion input");
		}
		else if(AInputEvent_getType(event)==AINPUT_EVENT_TYPE_KEY)
		{
			LOGW("Key input");
		}
	}

	static int32_t AEAndroidWindow::HandleCmd(struct android_app *app,int32_t cmd)
	{
		AEAndroidWindow *wnd=static_cast<AEAndroidWindow*>(app->userData);

		switch(cmd)
		{
		case APP_CMD_SAVE_STATE:
			// The system has asked us to save our current state.  Do so.
			// wnd->app->savedState = malloc(sizeof(struct saved_state));
			// *((struct saved_state*)wnd->app->savedState) = wnd->state;
			// wnd->app->savedStateSize = sizeof(struct saved_state);
			// wnd->engine->Suspend()
			LOGI("Save state");
			break;
		case APP_CMD_INIT_WINDOW:
			// The window is being shown, get it ready.
			if(wnd->app->window!=nullptr)
			{
				wnd->InitDisplay();
				LOGI("Init window");
			}
			break;
		case APP_CMD_TERM_WINDOW:
			// The window is being hidden or closed, clean it up.
			wnd->TerminateDisplay();
			LOGI("Terminate window");
			break;
		case APP_CMD_GAINED_FOCUS:
			this->active=true;
			LOGI("Window activated");
			break;
		case APP_CMD_LOST_FOCUS:
			this->active=false;
			LOGI("Window lost focus");
			break;
		}
	}

	int AEAndroidWindow::InitDisplay(void)
	{
		if(!app)
			return AE_ERR;

		// initialize OpenGL ES and EGL
		/*
		 * Here specify the attributes of the desired configuration.
		 * Below, we select an EGLConfig with at least 8 bits per color
		 * component compatible with on-screen windows
		 */
		const EGLint attribs[]={
				EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
				EGL_BLUE_SIZE, 8,
				EGL_GREEN_SIZE, 8,
				EGL_RED_SIZE, 8,
				EGL_NONE
		};
		EGLint w, h, dummy, format;
		EGLint numConfigs;
		EGLConfig config;

		display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

		eglInitialize(display,0,0);

		/* Here, the application chooses the configuration it desires. In this
		 * sample, we have a very simplified selection process, where we pick
		 * the first EGLConfig that matches our criteria */
		eglChooseConfig(display,attribs,&config,1,&numConfigs);

		/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
		 * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
		 * As soon as we picked a EGLConfig, we can safely reconfigure the
		 * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
		eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

		ANativeWindow_setBuffersGeometry(a_app->window, 0, 0, format);

		surface = eglCreateWindowSurface(display, config, a_app->window, NULL);
		context = eglCreateContext(display, config, NULL, NULL);

		if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
		{
			LOGW("Unable to eglMakeCurrent");
			return AE_ERR;
		}

		eglQuerySurface(display,surface,EGL_WIDTH,&w);
		eglQuerySurface(display,surface,EGL_HEIGHT,&h);

		if(width!=w||height!=h)
		{
			width=w;
			height=h;

			int param[5];
			param[0]=this->width;
			param[1]=this->height;
			this->CallEvent(AE_EVENT_RESIZE,param);
		}

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
	}

	int AEAndroidWindow::InitWindow(uint16_t _width,uint16_t _height,uint8_t _bpp,AE_WINDOW_TYPE _type)
	{
		AEWindow::InitWindow(_width,_height,_bpp,_type);

		return InitDisplay();
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