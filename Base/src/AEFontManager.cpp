/*
 * AEFontManager.cpp
 *
 *  Created on: Oct 18, 2012
 *      Author: klokik
 */

#include "AEFontManager.h"

AEFontManager::AEFontManager(AEMaterialManager* mat)
{
	this->material_manager=mat;
	this->_cuid=1;
}

int AEFontManager::GetUid(void)
{
	return _cuid++;
}

int AEFontManager::AddFont(AEFontBitmap font)
{
	int uid=GetUid();
	this->fonts.insert({uid,font});

	return uid;
}

int AEFontManager::LoadFont(const char *filename,const char *name,uint width,uint height)
{
	if(!material_manager||!filename) return 0;

	AEFontBitmap font;
	font.width=width;
	font.height=height;
	font.texture=NULL;
	font.name=name;

	if(material_manager->LoadTexture(font.texture,filename)<=0) return 0;

	return this->AddFont(font);
}

int AEFontManager::GetFontId(const char* name)
{
	for(std::pair<int,AEFontBitmap> item:this->fonts)
	{
		if(item.second.name==name)
			return item.first;
	}
	return 0;
}

void AEFontManager::SetMaterialManager(AEMaterialManager* mat)
{
	this->material_manager=mat;
}

AEFontManager::~AEFontManager(void)
{
	for(std::pair<int,AEFontBitmap> item:this->fonts)
	{
		this->material_manager->FreeTexture(item.second.texture);
	}
	this->fonts.clear();
}
