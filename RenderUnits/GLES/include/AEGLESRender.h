/*
 * AEGLESRender.h
 *
 *  Created on: Aug 4, 2013
 *      Author: klokik
 *
 *      Cross-platform rendering engine
 */

#ifndef AEGLESRENDER_H_
#define AEGLESRENDER_H_

#include "AEGLSLRender.h"


namespace aengine
{
	class AEGLESRenderUnit: public AEGLSLRenderUnit
	{
	protected:
		AEGLSLProgram2vsquare *p_blit;

		virtual void BlitToScreen(void) override;

		virtual int InitPrograms(void);
		virtual int InitBlitProgram(void);

	public:
		AEGLESRenderUnit(void);

		virtual ~AEGLESRenderUnit(void);
	};
}

#endif /* AEGLESRENDER_H_ */
