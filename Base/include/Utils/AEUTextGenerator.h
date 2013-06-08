/*
 * AEUTextGenerator.h
 *
 *  Created on: Dec 23, 2011
 *      Author: klokik
 */

#ifndef AEUTEXTGENERATOR_H_
#define AEUTEXTGENERATOR_H_

#include "AETexture.h"
#include "AETextBlock.h"
#include "AEMesh.h"

//generates texture from text object
class AEUTextGenerator
{
public:
//	AEUTextGenerator();

	virtual AETexture *GenTexture(AETextBlock &text)=0;
	virtual AEMesh *GenMesh(AETextBlock &text)=0;

	virtual ~AEUTextGenerator(){};//=0
};

#endif /* AEUTEXTGENERATOR_H_ */
