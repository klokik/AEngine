/*
 * AEObjectLight.cpp
 *
 *  Created on: Mar 13, 2012
 *      Author: klokik
 */

#include "AEObjectLight.h"


namespace aengine
{
	AEObjectLight::AEObjectLight(void)
	{
		this->Init();
		this->light_type=AE_POINT_LIGHT;
	}

	AEObjectLight::AEObjectLight(int ltype)
	{
		this->Init();
		this->light_type=ltype;
	}

	void AEObjectLight::Init(void)
	{
		this->_type=AE_OBJ_LIGHT;

		this->color.R=1.0f;
		this->color.G=1.0f;
		this->color.B=1.0f;
		this->color.A=1.0f;
		this->spot.X=20;
		this->spot.Y=1;
		this->attenuation={1.0f,1.0f,0.0f};
	}

	AEObjectLight::~AEObjectLight(void)
	{
	}
}
