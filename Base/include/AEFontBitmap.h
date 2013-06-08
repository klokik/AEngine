/*
 * AEFontBitmap.h
 *
 *  Created on: Oct 14, 2012
 *      Author: klokik
 */

#ifndef AEFONTBITMAP_H_
#define AEFONTBITMAP_H_

#include <string>

#include "AETexture.h"

typedef struct
{
	uint width; 	//number of tiles in one row
	uint height;	//number of tiles in one column

	AETexture *texture;

	std::string name;
} AEFontBitmap;

#endif /* AEFONTBITMAP_H_ */
