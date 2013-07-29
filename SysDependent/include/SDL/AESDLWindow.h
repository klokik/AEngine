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
#include "AEWindow.h"


namespace aengine
{
	class AESDLWindow: public AEWindow
	{
	protected:
		int vidFlags;
		const SDL_VideoInfo *vidInfo;
		SDL_Event event;
		SDL_Surface *screen;

		virtual void *WndProc(void *param) override;

	public:

		AESDLWindow(void *engine=nullptr);

		virtual int InitWindow(uint16_t _width,uint16_t _height,uint8_t _bpp,AE_WINDOW_TYPE _type);

		virtual void SetCursorPos(uint16_t x,uint16_t y) override;
		virtual void CenterCursor(void) override;

		virtual void SwapBuffers(void) override;

		virtual ~AESDLWindow(void);
	};
}

#endif /* AESDLWINDOW_H_ */
