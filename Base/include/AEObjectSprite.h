/*
 * AEObjectSprite.h
 *
 *  Created on: Dec 4, 2011
 *      Author: klokik
 */

#ifndef AEOBJECTSPRITE_H_
#define AEOBJECTSPRITE_H_

#include <stdint.h>

#include "AEObject.h"
#include "AEMaterial.h"

class AEObjectSprite: public AEObject
{
public:
	/* Note:
	 * Use rotate.X and rotate.Y to set pivot for sprite
	 * and rotate.Z for rotation,
	 * while using orthographic mode width and height are in pixels
	 * and in perspective mode it uses standard units
	 */

	AEMaterial *material;

	/* Default texture coordinates means that
	 * the whole image defined in material would be drawn on the sprite
	 * (0,1)  (1,1)
	 * +--------+
	 * |\       |
	 * |  \     |
	 * |    \   |
	 * |      \ |
	 * +--------+
	 * (0,0)  (1,0)
	 */
	AETexCoord tcr[4];
	bool tcr_usedefault;

	AEObjectSprite();

	virtual ~AEObjectSprite();
};

#endif /* AEOBJECTSPRITE_H_ */
