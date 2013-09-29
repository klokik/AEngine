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
	this->char_spacing=vec3f(0.8f,0.0f,0.0f);
	this->char_alignment=AE_CENTER;
	this->line_spacing=vec3f(0.0f,-1.7f,0.0f);
	this->line_alignment=AE_CENTER;
}
