/*
 * AEMaterial.h
 *
 *  Created on: Oct 9, 2011
 *      Author: klokik
 */

#ifndef AEMATERIAL_H_
#define AEMATERIAL_H_

#include "AEUnits.h"
#include "AETexture.h"

typedef struct AEMaterial
{
	AETexture *texture;

	AEColor diffuse;
	AEColor specular;
	AEColor emissive;

	float shininess;
	float reflectivity;

	bool shading;
}AEMaterial;

#endif /* AEMATERIAL_H_ */
