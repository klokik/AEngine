/*
 * AEScene.cpp
 *
 *  Created on: Sep 2, 2011
 *      Author: klokik
 */

#include <stddef.h>
#include <stdlib.h>
#include <algorithm>

#include "AEScene.h"
#include "AEDefines.h"


namespace aengine
{
	AEScene::AEScene(void)
	{
		fonts.SetMaterialManager(&this->materials);
	}

	void AEScene::AddObject(AEObject *obj)
	{
		this->objects.push_back(obj);

	//	if(obj->GetType()==AE_OBJ_LIGHT)
	//		this->lights.push_back((AEObjectLight*)obj);
	}

	void AEScene::RemoveObject(AEObject *obj)
	{
		this->objects.erase(std::remove(objects.begin(),objects.end(),obj),objects.end());
	}

	AEObject *AEScene::GetObject(std::string name)
	{
		for(auto object:objects)
			if(object->name==name)
				return object;
		
		return NULL;
	}

	AEScene::~AEScene(void)
	{
		for(unsigned int q=0;q<this->objects.size();q++)
			delete this->objects[q];
		this->objects.clear();
	}
}
