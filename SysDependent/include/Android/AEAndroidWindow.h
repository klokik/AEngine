/*
 * AESDLWindow.h
 *
 *  Created on: Jul 29, 2013
 *      Author: klokik
 */

#ifndef AEANDROIDWINDOW_H_
#define AEANDROIDWINDOW_H_

#include <EGL/egl.h>

#include "native_app_glue/android_native_app_glue.h"

#include "AEDefines.h"
#include "AEWindow.h"


namespace aengine
{
	class AEAndroidWindow: public AEWindow
	{
	protected:
		EGLDisplay display;
		EGLSurface surface;
		EGLContext context;

		struct android_app *a_app;

		bool first_init_call;
		bool break_after_init;

		virtual void *WndProc(void *param) override;

		static int32_t HandleInput(struct android_app *app,AInputEvent *event);
		static void HandleCmd(struct android_app *app,int32_t cmd);

		int InitDisplay(void);
		void TerminateDisplay(void);

	public:

		AEAndroidWindow(void *engine=nullptr);

		virtual int InitWindow(uint16_t _width,uint16_t _height,uint8_t _bpp,AE_WINDOW_TYPE _type);

		virtual void SetCursorPos(uint16_t x,uint16_t y) override;
		virtual void CenterCursor(void) override;

		virtual void SwapBuffers(void) override;

		virtual ~AEAndroidWindow(void);
	};
}

#endif /* AEANDROIDWINDOW_H_ */
