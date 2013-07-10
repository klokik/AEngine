/*
 * AEScene.h
 *
 *  Created on: Sep 2, 2011
 *      Author: klokik
 */

#ifndef AESCENE_H_
#define AESCENE_H_

#include <vector>

//#include "AECamera.h"
#include "AEObject.h"
#include "AEMaterialManager.h"
#include "AETimelineManager.h"
#include "AEFontManager.h"
#include "AEObjectLight.h"


class AEScene
{
protected:
	//std::vector<AEObjectLight*> lights;

public:
	std::vector<AEObject*> objects;

	AEMaterialManager materials;
	AETimelineManager animations;
	AEFontManager fonts;

	AEScene(void);

	void AddObject(AEObject *obj);
	void RemoveObject(AEObject *obj);

	AEObject *GetObject(std::string name);

	virtual ~AEScene(void);
};

#endif /* AESCENE_H_ */
