/*
 * AEMaterialManager.cpp
 *
 *  Created on: Oct 11, 2011
 *      Author: klokik
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//#include <SDL/SDL_image.h>

#include "AEMaterialManager.h"

AEMaterialManager::AEMaterialManager(void)
{
	this->IMGinit=false;
}

void AEMaterialManager::Add(AEMaterial * mat)
{
	this->materials.push_back(mat);
}

AEMaterial *AEMaterialManager::New(void)
{
	AEMaterial *mat=(AEMaterial*)malloc(sizeof(AEMaterial));

	mat->diffuse.R=0.5f; mat->diffuse.G=0.5f; mat->diffuse.B=0.5f; mat->diffuse.A=1.0f;
	mat->emissive.R=1.0f; mat->emissive.G=1.0f; mat->emissive.B=1.0f; mat->emissive.A=1.0f;
	mat->specular.R=1.0f; mat->specular.G=1.0f; mat->specular.B=1.0f; mat->specular.A=1.0f;
	mat->texture=nullptr;//(AETexture*)malloc(sizeof(AETexture));
	//memset(mat->texture,0,sizeof(AETexture));
	mat->reflectivity=0;
	mat->shininess=0.5f;
	mat->shading=true;
	mat->transparent=false;

	this->Add(mat);
	return mat;
}

// int AEMaterialManager::LoadTexture(AETexture *&tex,const char *filename)
// {
// 	// if(!this->IMGinit)
// 	// {
// 	// 	if(!IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG))
// 	// 		return -1;
// 	// 		else IMGinit=true;
// 	// }

// //	SDL_Surface *surf=IMG_Load(filename);

// //	if(!surf) return -2;

// 	if(tex)
// 	{
// 		if(tex->size>0)
// 			free(tex->data);
// 	}
// 	else
// 		tex=(AETexture*)malloc(sizeof(AETexture));

// 	tex->width=256;//surf->w;
// 	tex->height=256;//surf->h;

// 	//tex.pixelformat=surf->format;
// 	tex->bpp=32;//surf->pitch/surf->w*8;
// 	tex->size=tex->width*tex->height*tex->bpp/8;//surf->pitch*surf->h;

// 	tex->data=malloc(tex->size);
// 	//copy and flip texture vertically
// //	memcpy(tex->data,surf->pixels,tex->size);
// 	// for(size_t q=0;q<surf->h;q++)
// 	// {
// 		// memcpy(tex->data+q*surf->w,surf->pixels+q*surf->w,surf->w);
// 	// }
// 	// size_t bypp = tex->bpp/8;
// 	// for(size_t q=0;q<tex->size;q+=bypp)
// 	// {
// 	// 	((uint8_t*)tex->data)[q]=((uint8_t*)surf->pixels)[tex->size-q-1-2];
// 	// 	((uint8_t*)tex->data)[q+1]=((uint8_t*)surf->pixels)[tex->size-q-1-1];
// 	// 	((uint8_t*)tex->data)[q+2]=((uint8_t*)surf->pixels)[tex->size-q-1];
// 	// }

// //	SDL_FreeSurface(surf);

// 	printf("Texture loaded %dx%d\n",tex->width,tex->height);

// 	return 1;
// }

AEMaterial *AEMaterialManager::operator[](size_t id) const
{
	return this->materials[id];
}

size_t AEMaterialManager::Count() const
{
	return this->materials.size();
}

void AEMaterialManager::Delete(AEMaterial *mat)
{
	puts("AEMaterialManager::Delete() - Not implemented");
	//TODO:Remove material from list
}

void AEMaterialManager::OptimizeTexture(AETexture *&tex)
{
	if(tex==NULL) return;
	if(tex->size>0) { free(tex->data); tex->size=0; }
}

void AEMaterialManager::FreeTexture(AETexture *&tex)
{
	this->OptimizeTexture(tex);
	free(tex);	//use carefull
	tex=nullptr;
}

AEMaterialManager::~AEMaterialManager(void)
{
	for(AEMaterial *mat:this->materials)
		this->OptimizeTexture(mat->texture);	//free memory kept by texture but not remove texture itself, as it can be used by several materials

	for(AEMaterial *mat:this->materials)
	{
		this->FreeTexture(mat->texture);		//now we can completely free texture;
		free(mat);
	}

	materials.clear();
}

