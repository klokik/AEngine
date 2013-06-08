/*
 * AEGLSLProgramManager.h
 *
 *  Created on: Jan 7, 2012
 *      Author: klokik
 */

#ifndef AEGLSLPROGRAMMANAGER_H_
#define AEGLSLPROGRAMMANAGER_H_

#include <vector>

#include "AEGLSLProgram.h"

class AEGLSLProgramManager
{
protected:
	std::vector<AEGLSLShader*> shaders;
	std::vector<AEGLSLProgram*> programs;

public:
	AEGLSLProgramManager(void);

	AEGLSLProgram *NewProgram(void);
	void ReleaseProgram(AEGLSLProgram *program);

	AEGLSLShader *NewShader(uint32_t type);
	void ReleaseShader(AEGLSLShader *shader);

	void Free(void);

	virtual ~AEGLSLProgramManager(void);
};

#endif /* AEGLSLPOGRAMMANAGER_H_ */
