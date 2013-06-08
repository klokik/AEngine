/*
 * AEObjectText.cpp
 *
 *  Created on: Oct 13, 2012
 *      Author: klokik
 */

#include "AEObjectText.h"
#include "AEDefines.h"
#include "AEVectorMath.h"

AEObjectText::AEObjectText(void)
{
	this->_type=AE_OBJ_TEXT;
	this->projection=AE_ORTHOGRAPHIC;
	this->SetScale(vec3f(16.0f,16.0f,16.0f));
	this->spacing={1.1f,0.0f,0.0f};
	this->alignment=AE_CENTER;
}
