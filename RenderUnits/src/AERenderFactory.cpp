/*
 * AERenderFactory.cpp
 *
 *  Created on: Feb 1, 2012
 *      Author: klokik
 */

#include <string.h>

#include "render_build_config.h"


#if defined(BUILD_GL_RENDER)
	#include "AEGLRender.h"
#endif

#if defined(BUILD_GLSL_RENDER)
	#include "AEGLSLRender.h"
#endif

#if defined(BUILD_GLES_RENDER)
 	#include "AEGLSLRender.h"
#endif

#include "AERenderFactory.h"

AERenderFactory::AERenderFactory()
{
	this->units.push_back("GL");
	this->units.push_back("GLSL");
	this->units.push_back("GLES");
}

const std::vector<std::string> &AERenderFactory::ListAvailableRenderUnits(void)
{
	return this->units;
}

AERenderUnit *AERenderFactory::GetRenderUnit()
{
	if(this->units.size()>0)
		return this->GetRenderUnit(this->units[0].c_str());
	else
		return NULL;
}

AERenderUnit *AERenderFactory::GetRenderUnit(const char *type)
{
	uint32_t _type=0;

	if(!strncmp(type,"GL",2))	_type=AE_RENDER_GL;
	if(!strncmp(type,"GLSL",4))	_type=AE_RENDER_GLSL;
	if(!strncmp(type,"GLES",4))	_type=AE_RENDER_GLES;
	if(!strncmp(type,"D3D",3))	_type=AE_RENDER_D3D;

	return this->GetRenderUnit(_type);
}

AERenderUnit *AERenderFactory::GetRenderUnit(uint32_t type)
{
	switch(type)
	{
#if defined(BUILD_GL_RENDER)
	case AE_RENDER_GL:
		return new AEGLRenderUnit;
#endif

#if defined(BUILD_GLSL_RENDER)
	case AE_RENDER_GLSL:
		return new AEGLSLRenderUnit;
#endif

#if defined(BUILD_GLES_RENDER)
	case AE_RENDER_GLES:
		return new AEGLESRenderUnit;
#endif

	case AE_RENDER_D3D:
		return NULL;

	default:
		return NULL;
	}
}

AERenderFactory::~AERenderFactory()
{
}

