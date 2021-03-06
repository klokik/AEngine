/*
 * AEObjectText.h
 *
 *  Created on: Oct 13, 2012
 *      Author: klokik
 */

#ifndef AEOBJECTTEXT_H_
#define AEOBJECTTEXT_H_

#include <string>

#include "AEObject.h"


class AEObjectText: public AEObject
{
public:
	AEObjectText(void);

	std::string text;

	AEVector3f char_spacing;
	AEVector3f line_spacing;
	unsigned int char_alignment;
	unsigned int line_alignment;
};

#endif /* AEOBJECTTEXT_H_ */
