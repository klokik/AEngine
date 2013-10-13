/*
 * AERender.h
 *
 *  Created on: Sep 2, 2011
 *      Author: klokik
 *
 *      Abstract class for rendering engine
 */

#ifndef AERENDER_H_
#define AERENDER_H_

#include <stdint.h>

#include "AEScene.h"
#include "AEObjectCamera.h"

// TODO: wrap render engines into namespace
using namespace aengine;

class AERenderUnit
{
protected:
	uint16_t width;	//Use this for storing current viewport size
	uint16_t height;
public:
	//AERenderBase(){};

	virtual int Init(uint16_t _width,uint16_t _height)=0;
	virtual void Resize(uint16_t _width,uint16_t _height)=0;

	virtual void CacheScene(AEScene *scene)=0;
	virtual void CacheClear(void)=0;

//	virtual void RenderObject(const AEObject *obj)=0;
	virtual void Render(AEObjectCamera *camera)=0;

	virtual ~AERenderUnit(void){};
};

#endif /* AERENDER_H_ */
