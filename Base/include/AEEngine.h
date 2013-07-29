/*
 * AEEngine.h
 *
 *  Created on: Nov 8, 2011
 *      Author: klokik
 */

#ifndef AEENGINE_H_
#define AEENGINE_H_

#include "AEWindow.h"
#include "AERender.h"
#include "AEScene.h"
#include "AEObjectCamera.h"

namespace aengine
{
	class AEEngine
	{
	protected:
		unsigned char keys[323];

	public:
		AEScene			*scene;
		AERenderUnit	*render;
		AEWindow		*window;

		AEObjectCamera	*curCamera;

		//Internal window event's implementation
		// for details look to "AEWindow.h"
		virtual void i_OnClose(int *param);
		virtual void i_OnKeyDown(int *param);
		virtual void i_OnKeyUp(int *param);
		virtual void i_OnMouseDown(int *param);
		virtual void i_OnMouseMove(int *param);
		virtual void i_OnMouseUp(int *param);
		virtual void i_OnResize(int *param);
		virtual void i_OnStart(int *param);

		virtual void i_Refresh(int *param);

		//External event pointers
		AE_EVENT OnClose;			//attributes {NULL}
		AE_EVENT OnKeyDown;			//attributes {int keycode}
		AE_EVENT OnKeyUp;			//attributes {int keycode}
		AE_EVENT OnMouseDown;		//attributes {int x,y,mousekey}
		AE_EVENT OnMouseMove;		//attributes {int x,y,deltaX,deltaY,keys}
		AE_EVENT OnMouseUp;			//attributes {int x,y,mousekey}
		AE_EVENT OnResize;			//attributes {int width,height}
		AE_EVENT OnStart;			//attributes {NULL}

		AE_EVENT Refresh;			//attirbutes {int interval}

		AEEngine(void);

		virtual int Init(uint32_t flags);

		virtual int Run(void);
		virtual int Stop(void);

		virtual unsigned char GetKeyState(AE_KEY key) const;

		virtual ~AEEngine(void);
	};
}

#endif /* AEENGINE_H_ */
