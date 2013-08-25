/*
 * AEGLSLProgram.cpp
 *
 *  Created on: Mar 17, 2012
 *      Author: klokik
 */

#include "AEGLHeader.h"
#include "AEGLSLProgram.h"
#include "AEDebug.h"
#include "AEDefines.h"
#include <stdio.h>


namespace aengine
{
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

	void AEGLSLShader::ShaderData(std::string data)
	{
		const char *c_data=data.data();
		glShaderSource(this->id,1,&c_data,nullptr);
	}

	int AEGLSLShader::Compile(bool quiet)
	{
		glCompileShader(this->id);

		if(GetCompileStatus()!=GL_TRUE)
		{
			if(!quiet)
				AEPrintLog(GetLog());
			return AE_ERR;
		}

		return AE_OK;
	}

	int AEGLSLShader::GetCompileStatus(void)
	{
		int result;
		glGetShaderiv(this->id,GL_COMPILE_STATUS,&result);
		char buf[256];
		sprintf(buf,"compile status: %d",result);
		AEPrintLog(buf);
		return result;
	}

	std::string AEGLSLShader::GetLog(void)
	{
		GLint mlength;
		glGetShaderiv(this->id,GL_INFO_LOG_LENGTH,&mlength);
		char *buf=(char*)malloc(mlength*sizeof(char));
		glGetShaderInfoLog(this->id,mlength,nullptr,buf);
		std::string str;
		str.append(buf);
		free(buf);
		return str;
	}

	AEGLSLShader::~AEGLSLShader(void)
	{
		if(this->id)
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

	void AEGLSLProgram::Attach(AEGLSLShader &shd)
	{
		glAttachShader(this->id,shd.id);
	}

	void AEGLSLProgram::Detach(AEGLSLShader &shd)
	{
		glDetachShader(this->id,shd.id);
	}

	int AEGLSLProgram::Link(bool quiet)
	{
		glLinkProgram(this->id);

		if(GetLinkStatus()!=GL_TRUE)
		{
			if(!quiet)
				AEPrintLog(GetLog());
			return AE_ERR;
		}

		return AE_OK;
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
}
