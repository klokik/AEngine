/*
 * AEGLSLProgram.cpp
 *
 *  Created on: Mar 17, 2012
 *      Author: klokik
 */

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

#include <GL/gl.h>

#include "AEGLSLProgram.h"

AEGLSLProperty::AEGLSLProperty(void)
{
	this->id=0;
	this->type=0;
}

AEGLSLShader::AEGLSLShader(uint32_t _type)
{
	this->type=_type;
	this->id=glCreateShader(type);
}

void AEGLSLShader::ShaderData(const char **str, uint32_t count, const int *length)
{
	glShaderSource(this->id,count,str,length);
}

void AEGLSLShader::Compile(void)
{
	glCompileShader(this->id);
}

int AEGLSLShader::GetCompileStatus(void)
{
	int result;
	glGetShaderiv(this->id,GL_COMPILE_STATUS,&result);
	return result;
}

std::string AEGLSLShader::GetLog(void)
{
	int mlength,length;
	glGetShaderiv(this->id,GL_INFO_LOG_LENGTH,&mlength);
	char *buf=(char*)malloc(mlength*sizeof(char));
	glGetShaderInfoLog(this->id,mlength,&length,buf);
	std::string str;
	str.append(buf,length);
	free(buf);
	return str;
}

AEGLSLShader::~AEGLSLShader(void)
{
	glDeleteShader(this->id);
}

AEGLSLProgram::AEGLSLProgram(void)
{
	this->id=glCreateProgram();
}

void AEGLSLProgram::Use(void)
{
	glUseProgram(this->id);
}

void AEGLSLProgram::Attach(AEGLSLShader *shd)
{
	glAttachShader(this->id,shd->id);
}

void AEGLSLProgram::Detach(AEGLSLShader *shd)
{
	glDetachShader(this->id,shd->id);
}

void AEGLSLProgram::Link(void)
{
	glLinkProgram(this->id);
}

int AEGLSLProgram::GetLinkStatus(void)
{
	int result;
	glGetProgramiv(this->id,GL_LINK_STATUS,&result);
	return result;
}

std::string AEGLSLProgram::GetLog(void)
{
	int mlength,length;
	glGetProgramiv(this->id,GL_INFO_LOG_LENGTH,&mlength);
	char *buf=(char*)malloc(mlength*sizeof(char));
	glGetProgramInfoLog(this->id,mlength,&length,buf);
	std::string str;
	str.append(buf,length);
	free(buf);
	return str;
}

int AEGLSLProgram::GetUniform(const char *name)
{
	return glGetUniformLocation(this->id,name);
}

int AEGLSLProgram::GetAttribute(const char *name)
{
	return glGetAttribLocation(this->id,name);
}

void AEGLSLProgram::GetUniform(AEGLSLProperty & prop)
{
	prop.id=GetUniform(prop.name.c_str());
}

void AEGLSLProgram::GetAttribute(AEGLSLProperty & prop)
{
	prop.id=GetAttribute(prop.name.c_str());
}

void AEGLSLProgram::GetShaderProperties(void)
{
	//nothing to do here
}

AEGLSLProgram::~AEGLSLProgram(void)
{
	glDeleteProgram(this->id);
}
