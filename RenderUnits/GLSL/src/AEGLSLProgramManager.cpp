/*
 * AEGLSLProgramManager.cpp
 *
 *  Created on: Jan 7, 2012
 *      Author: klokik
 */

#include <algorithm>

#include "AEGLSLProgramManager.h"
#include "AEDebug.h"


namespace aengine
{
	AEGLSLProgramManager::AEGLSLProgramManager()
	{
	}

	void AEGLSLProgramManager::Free(void)
	{
		// Call GarbageCollector here

		for(AEGLSLShader *&shd:this->shaders)
			if(*(int*)shd) delete shd;
		for(AEGLSLProgram *&prg:this->programs)
			if(*(int*)prg) delete prg;

		this->shaders.clear();
		this->programs.clear();
	}

	AEGLSLProgram *AEGLSLProgramManager::NewProgram(void)
	{
		AEGLSLProgram *prg=new AEGLSLProgram;
		if(prg->id==0)
		{
			AEPrintLog("Failed to create shader object");
			delete prg;
			return nullptr;
		}
		this->programs.push_back(prg);
		return prg;
	}

	void AEGLSLProgramManager::ReleaseProgram(AEGLSLProgram *program)
	{
		std::vector<AEGLSLProgram*>::iterator pos=std::find(programs.begin(),programs.end(),program);
		if(pos!=programs.end())
			programs.erase(pos);
		delete program;
	}

	AEGLSLShader *AEGLSLProgramManager::NewShader(uint32_t type)
	{
		AEGLSLShader *shd=new AEGLSLShader(type);

		this->shaders.push_back(shd);
		return shd;
	}

	void AEGLSLProgramManager::ReleaseShader(AEGLSLShader *shader)
	{
		std::vector<AEGLSLShader*>::iterator pos=std::find(shaders.begin(),shaders.end(),shader);
		if(pos!=shaders.end())
			shaders.erase(pos);
		delete shader;
	}

	AEGLSLProgramManager::~AEGLSLProgramManager()
	{
		this->Free();
	}
}
