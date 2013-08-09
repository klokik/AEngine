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

#include "AERender.h"


namespace aengine
{
	class AEGLESRenderUnit: public AERenderUnit
	{
	public:
		virtual int Init(uint16_t _width,uint16_t _height);
		virtual void Resize(uint16_t _width,uint16_t _height);

		virtual void CacheScene(AEScene *scene);
		virtual void CacheClear(void);

		virtual void Render(AEObjectCamera *camera);

		virtual ~AEGLESRenderUnit(void);
	};
}

#endif /* AEGLESRENDER_H_ */
