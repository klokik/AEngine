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
			LOGI("POP");
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
				LOGI("Looper_looop");
			}

			if(this->break_after_init)
			{
				this->break_after_init=false;
				LOGI("Window initalization complete");
				break;
			}

			if(active)
			{
				// static float iter=0.0f;
				// glClearColor(iter,1.0f-iter,(iter-0.5f)*0.3f+0.5f,1.0f);
				// glClear(GL_COLOR_BUFFER_BIT);
				// iter+=0.05f;
				// if(iter>1.0f)
				// 	iter=0.0f;
				LOGI("Begin refresh");
				this->CallEvent(AE_REFRESH,param);
				LOGI("End refresh");
			}
		}

		return nullptr;
	}

	int32_t AEAndroidWindow::HandleInput(struct android_app *app,AInputEvent *event)
	{
		if(AInputEvent_getType(event)==AINPUT_EVENT_TYPE_MOTION)
		{
			LOGI("Motion input");
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

		LOGI("Handle");
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
		}
		LOGI("Still alive");
	}

	int AEAndroidWindow::InitDisplay(void)
	{
		if(!a_app)
		{
			LOGW("Something's wrong with world::a_app");
			return AE_ERR;
		}

		// // initialize OpenGL ES and EGL
		// /*
		//  * Here specify the attributes of the desired configuration.
		//  * Below, we select an EGLConfig with at least 8 bits per color
		//  * component compatible with on-screen windows
		//  */
		// const EGLint attribs[]={
		// 		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		// 		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		// 		EGL_BUFFER_SIZE, 32,
		// 		EGL_ALPHA_SIZE, 8,
		// 		EGL_BLUE_SIZE, 8,
		// 		EGL_GREEN_SIZE, 8,
		// 		EGL_RED_SIZE, 8,
		// 		EGL_DEPTH_SIZE, 24,
		// 		EGL_STENCIL_SIZE, 8,
		// 		EGL_NONE
		// };
		// const EGLint attrib_list[]={
		// 	EGL_CONTEXT_CLIENT_VERSION, 2,
		// 	EGL_NONE
		// };
		// EGLint w, h, format;
		// EGLint numConfigs;
		// EGLConfig config;

		// display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

		// eglInitialize(display,0,0);

		// /* Here, the application chooses the configuration it desires. In this
		//  * sample, we have a very simplified selection process, where we pick
		//  * the first EGLConfig that matches our criteria */
		// eglChooseConfig(display,attribs,&config,1,&numConfigs);

		// LOGW("Num configs: %d",numConfigs);

		// /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
		//  * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
		//  * As soon as we picked a EGLConfig, we can safely reconfigure the
		//  * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
		// eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

		// ANativeWindow_setBuffersGeometry(a_app->window, 0, 0, format);

		// surface = eglCreateWindowSurface(display, config, a_app->window, NULL);
		// if(surface==EGL_NO_SURFACE)
		// {
		// 	LOGW("Unable to create surface");
		// 	return AE_ERR;
		// }

		// context = eglCreateContext(display, config, EGL_NO_CONTEXT, attrib_list);
		// if(context==EGL_NO_CONTEXT)
		// {
		// 	LOGW("Unable to create context");
		// 	return AE_ERR;
		// }

		// if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
		// {
		// 	LOGW("Unable to eglMakeCurrent");
		// 	return AE_ERR;
		// }

			// initialize OpenGL ES and EGL


		const EGLint attribs[] = 
		{
			EGL_NATIVE_VISUAL_ID, WINDOW_FORMAT_RGB_565,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
			EGL_BLUE_SIZE, 5,
			EGL_GREEN_SIZE, 6,
			EGL_RED_SIZE, 5,
			EGL_DEPTH_SIZE,1,
			EGL_NONE
		};

		EGLint w, h, format;
		EGLSurface surface;
		EGLContext context;

		EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);


		EGLint majorVersion;
		EGLint minorVersion;

		eglInitialize(display, &majorVersion, &minorVersion);
		//eglInitialize(display, 0, 0);
		LOGI("EGL %i.%i", majorVersion,minorVersion);

		//query num of configs
		int* num_conf = new int[1];
		eglGetConfigs(display, NULL, 0, num_conf);  //if configuration array is null it still returns the number of configurations
		int configurations = num_conf[0];

		LOGI("total num configs: %i", configurations);

		//just some debugging info if the need arises...
		LOGI("EGL_OPENGL_ES2_BIT id:%i", EGL_OPENGL_ES2_BIT); //print the numerical code for the ES2 bit mask, etc
		LOGI("EGL_SURFACE_TYPE::EGL_WINDOW_BIT id:%i", EGL_WINDOW_BIT);
		LOGI("WINDOW_FORMAT_RGB_565 id:%i", WINDOW_FORMAT_RGB_565);

		//now query the configs
		EGLConfig* conf = new EGLConfig[configurations];
		eglGetConfigs(display, conf, configurations, num_conf);

		int* depth = new int[1];
		int* r = new int[1];
		int* g = new int[1];
		int* b = new int[1];
		int* a = new int[1];
		int* s = new int[1];
		int* renderType = new int[1];
		int* surfaceType = new int[1];
		int* formatType = new int[1];

		EGLConfig configToUse; //this is the one true config that we will use

		for(int i = 0; i < configurations; i++)
		 {
		  eglGetConfigAttrib(display, conf[i], EGL_DEPTH_SIZE, depth);
		  eglGetConfigAttrib(display, conf[i], EGL_RED_SIZE, r);
		  eglGetConfigAttrib(display, conf[i], EGL_GREEN_SIZE, g);
		  eglGetConfigAttrib(display, conf[i], EGL_BLUE_SIZE, b);
		  eglGetConfigAttrib(display, conf[i], EGL_ALPHA_SIZE, a);
		  eglGetConfigAttrib(display, conf[i], EGL_RENDERABLE_TYPE, renderType);
		  eglGetConfigAttrib(display, conf[i], EGL_STENCIL_SIZE, s);
		  eglGetConfigAttrib(display, conf[i], EGL_SURFACE_TYPE, surfaceType);
		  eglGetConfigAttrib(display, conf[i], EGL_NATIVE_VISUAL_ID, formatType);

		  LOGI("(R%i,G%i,B%i,A%i)depth:(%i) stencil:(%i) surfaceType:(%i) renderType:(%i) formatType:(%i)",r[0],g[0],b[0],a[0],depth[0],s[0],surfaceType[0], renderType[0],formatType[0]);


		  if((renderType[0] & EGL_OPENGL_ES2_BIT) > 0 &&
			(surfaceType[0] & EGL_WINDOW_BIT) > 0 &&
			(formatType[0] & WINDOW_FORMAT_RGB_565) > 0 &&
			depth[0]>0)
		  {

			  configToUse=conf[i];

			  LOGI("Config #%i" , i );

			  LOGI("(R%i,G%i,B%i,A%i) %idepth %istencil %isurfaceType %iNativeVisualId",r[0],g[0],b[0],a[0],depth[0],s[0],surfaceType[0],formatType[0]);
		  }

		 }

		//bridge the pixel format back into android
		eglGetConfigAttrib(display, configToUse, EGL_NATIVE_VISUAL_ID, &format);
		ANativeWindow_setBuffersGeometry(a_app->window, 0, 0, format);

		surface = eglCreateWindowSurface(display, configToUse, a_app->window, NULL);

		if(surface == EGL_NO_SURFACE ) 
		{
			LOGW("Error making surface, EGL_NO_SURFACE");
		}

		//now create the OpenGL ES2 context
		const EGLint contextAttribs[] = 
		{
				EGL_CONTEXT_CLIENT_VERSION , 2,
				EGL_NONE
		};

		context = eglCreateContext(display, configToUse, NULL, contextAttribs);

		if(context == EGL_NO_CONTEXT ) 
		{
			LOGW("Error making context, EGL_NO_CONTEXT");
		}

		if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) 
		{
			LOGW("Unable to eglMakeCurrent");
			return -1;
		}

		eglQuerySurface(display, surface, EGL_WIDTH, &w);
		eglQuerySurface(display, surface, EGL_HEIGHT, &h);

		this->display = display;
		this->context = context;
		this->surface = surface;
		this->width = w;
		this->height = h;

		glEnable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		//GLubyte* rendererString = new GLubyte[512];
		const GLubyte* rendererString=glGetString(GL_VERSION);
		LOGI("Renderer: %s",rendererString);


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
	}

	int AEAndroidWindow::InitWindow(uint16_t _width,uint16_t _height,uint8_t _bpp,AE_WINDOW_TYPE _type)
	{
		AEWindow::InitWindow(_width,_height,_bpp,_type);

		LOGI("Init window");

		this->break_after_init=false;
		this->WndProc(nullptr);

		LOGI("Done: returned");

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
		LOGI("SwapBuffers");
		eglSwapBuffers(display,surface);
		LOGI("EndSwap");
	}

	AEAndroidWindow::~AEAndroidWindow(void)
	{
		TerminateDisplay();
		LOGI("Window destroyed");
	}
}