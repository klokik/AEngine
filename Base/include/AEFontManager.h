/*
 * AEFontManager.h
 *
 *  Created on: Oct 18, 2012
 *      Author: klokik
 */

#ifndef AEFONTMANAGER_H_
#define AEFONTMANAGER_H_

#include <map>

#include "AEFontBitmap.h"
#include "AEMaterialManager.h"

class AEFontManager
{
protected:
	int _cuid;
	AEMaterialManager *material_manager;	//TODO Replace with texture manager

	int GetUid(void);
public:
	std::map<int,AEFontBitmap> fonts;

	AEFontManager(AEMaterialManager *mat=NULL);

	void SetMaterialManager(AEMaterialManager *mat);
	int AddFont(AEFontBitmap font);
	int LoadFont(const char *filename,const char *name,uint width,uint height);
	int GetFontId(const char *name);

	~AEFontManager(void);
};

#endif /* AEFONTMANAGER_H_ */
