/*
 * AELightingCache.cpp
 *
 *  Created on: Mar 18, 2012
 *      Author: klokik
 */

#include <stddef.h>
#include <math.h>

#include "AELightingCache.h"
#include "AEObject.h"
#include "AEObjectLight.h"
#include "AEVectorMath.h"


namespace aengine
{
	AELightingCache::AELightingCache()
	{
		arr_length=0;
	}

	void AELightingCache::BuildCache(const std::vector<AEObject*> & objects)
	{
		//this->lights.clear();
		for(size_t q=0;q<objects.size();q++)
		{
			if(objects[q]->type==AE_OBJ_LIGHT)
				this->lights.push_back((AEObjectLight*)objects[q]);
			this->BuildCache(objects[q]->children);
		}
	}

	void AELightingCache::BuildRelativeArray(AEObject *obj)
	{
		arr_length=0;
		arr_type.clear();
		arr_position.clear();
		arr_rotation.clear();
		arr_color.clear();
		arr_attenuation.clear();
		arr_spot.clear();

		AEMatrix4f4 obj_inverse=obj->GetWorldMatrix().Invert();

		size_t max=lights.size();
		for(size_t q=0;q<max;q++)
		{
			AEObjectLight *light=lights[q];
			arr_type.push_back(light->light_type);
			// AEVector3f pos=light->translate;
			// if(obj)
			// {
			// 	pos.X-=obj->translate.X;
			// 	pos.Y-=obj->translate.Y;
			// 	pos.Z-=obj->translate.Z;
			// }
			AEVector4f ppos = vec4f(light->translate,1.0f);
			if(obj)
			{
				ppos = obj_inverse*ppos;
			}
			arr_position.push_back(vec3f(ppos.X,ppos.Y,ppos.Z));
			arr_color.push_back(light->color);
			arr_attenuation.push_back(light->attenuation);
			arr_spot.push_back(light->spot);

			//Multiplicate object's world matrix on vector {0,-1,0,0} to get normalised light vector
			AEVector4f rot4;
			rot4=lights[q]->GetWorldMatrix()*vec4f(0,-1,0,0);
			if(obj)
				rot4=obj_inverse*rot4;

			arr_rotation.push_back(vec3f(rot4.X,rot4.Y,rot4.Z));

			arr_length++;
		}
	}

	void AELightingCache::BuildArray(void)
	{
		this->BuildRelativeArray(NULL);
	}

	AELightingCache::~AELightingCache()
	{
		this->lights.clear();

		arr_type.clear();
		arr_position.clear();
		arr_rotation.clear();
		arr_color.clear();
		arr_attenuation.clear();
		arr_spot.clear();
	}
}
