/*
 * AEMaterialManager.h
 *
 *  Created on: Oct 11, 2011
 *      Author: klokik
 */

#ifndef AEMATERIALMANAGER_H_
#define AEMATERIALMANAGER_H_

#include <stdint.h>
#include <vector>

#include "AEMaterial.h"

class AEMaterialManager
{
private:
	std::vector<AEMaterial*> materials;

	bool IMGinit;

public:
	AEMaterialManager(void);

	void Add(AEMaterial *mat);
	AEMaterial *New(void);
	void Delete(AEMaterial *mat);

	AEMaterial *operator[](size_t id) const;
	size_t Count() const;

	int LoadTexture(AETexture *&tex,const char *filename);
	void OptimizeTexture(AETexture *tex);
	void FreeTexture(AETexture *tex);

	virtual ~AEMaterialManager(void);
};

#endif /* AEMATERIALMANAGER_H_ */
