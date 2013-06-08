/*
 * AEObjectSprite.cpp
 *
 *  Created on: Dec 4, 2011
 *      Author: klokik
 */

#include "AEObjectSprite.h"


AEObjectSprite::AEObjectSprite()
{
	this->_type=AE_OBJ_SPRITE;
	this->projection=AE_ORTHOGRAPHIC;
	this->material=NULL;
	this->tcr_usedefault=true;
}

AEObjectSprite::~AEObjectSprite()
{
}

