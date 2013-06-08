/*
 * AELightingCache.h
 *
 *  Created on: Mar 18, 2012
 *      Author: klokik
 */

#ifndef AELIGHTINGCACHE_H_
#define AELIGHTINGCACHE_H_

#include <vector>

#include "AEObjectLight.h"

typedef struct
{
	AE_LIGHT_TYPE type;
	AEVector3f translate;
	AEVector3f rotate;	//WARNING it's not angle it's normalized vector
	AEColor color;
	AEVector2f spot;	//x-angle, y-exponent
}AELight;

class AELightingCache
{
protected:
	std::vector<AEObjectLight*> lights;

public:
	size_t arr_length;
	std::vector<AE_LIGHT_TYPE> arr_type;
	std::vector<AEVector3f> arr_position;
	std::vector<AEVector3f> arr_rotation;
	std::vector<AEColor> arr_color;
	std::vector<AEVector3f> arr_attenuation;
	std::vector<AEVector2f> arr_spot;

	AELightingCache(void);

	void BuildCache(const std::vector<AEObject*> &objects);
	void BuildArray(void);
	void BuildRelativeArray(AEObject *obj);	//position of light is relative to passed object, usually current camera

	virtual ~AELightingCache(void);
};

#endif /* AELIGHTINGCACHE_H_ */
