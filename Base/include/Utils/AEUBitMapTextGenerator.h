/*
 * AEUBitMapTextGenerator.h
 *
 *  Created on: Dec 24, 2011
 *      Author: klokik
 */

#ifndef AEUBITMAPTEXTGENERATOR_H_
#define AEUBITMAPTEXTGENERATOR_H_

#include "AEUTextGenerator.h"
#include "AEMesh.h"

class AEUBitMapTextGenerator: public AEUTextGenerator
{
protected:
	virtual void PutItem(AETexture *tex,_aet_item item);

public:
	AETexture *map;

	uint8_t width;
	uint8_t height;

	char *layout;

	AEUBitMapTextGenerator();

	virtual AETexture *GenTexture(AETextBlock &text);
	virtual AEMesh *GenMesh(AETextBlock &text);

	virtual ~AEUBitMapTextGenerator();
};

#endif /* AEUBITMAPTEXTGENERATOR_H_ */
