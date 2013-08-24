/*
 * AETexture.h
 *
 *  Created on: Oct 9, 2011
 *      Author: klokik
 */

#ifndef AETEXTURE_H_
#define AETEXTURE_H_

#define AE_PF_RGB	0x01;
#define AE_PF_RGBA	0x02;
#define AE_PF_ARGB	0x03;
#define AE_PF_BGR	0x04;
#define AE_PF_BGRA	0x05;
#define AE_PF_ABGR	0x06;

#include <stdint.h>

typedef struct
{
	uint32_t id;
	uint16_t width;
	uint16_t height;

	uint8_t	pixelformat;
	uint8_t	bpp;
	uint32_t	size;		//data size in bytes

	void *data;
}AETexture;

typedef AETexture AEImage;

#endif /* AETEXTURE_H_ */
