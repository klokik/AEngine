/*
 * AEGLCache.cpp
 *
 *  Created on: Feb 12, 2012
 *      Author: klokik
 */

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

#include <GL/gl.h>

#include "AEGLRender.h"
#include "AEObjectMesh.h"

void AEGLRenderUnit::CacheScene(AEScene * _scene)
{
	this->scene=_scene;

	for(size_t q=0;q<this->scene->objects.size();q++)
	{
		this->CacheObject(this->scene->objects[q]);
	}
//
//	this->type_cache.BuildCache(_scene);
//
//	for(size_t q=0;q<type_cache.meshes.size();q++)
//	{
//		if(!this->CacheMesh(type_cache.meshes[q].mesh->mesh)) break;
//	}
//
	this->CacheMaterials();
	this->CacheFonts();
	//FIXME: avoid of appearing light objects twice in light cache
	this->lighting_cache.BuildCache(this->scene->objects);

	this->cached=true;
}

void AEGLRenderUnit::CacheClear(void)
{
	// Clear buffers and buffer-index array

	glDeleteBuffers(this->buffers.size(),&this->buffers[0]);
	glDeleteTextures(this->textures.size(),&this->textures[0]);

	//TODO Add mesh manager that would store all meshes and clear/cache them from it

//	for(size_t q=0;q<type_cache.meshes.size();q++)
//	{
//		type_cache.meshes[q].mesh->mesh->cached=false;
//	}
//
//	this->type_cache.Clear();
	this->cached=false;
}

int AEGLRenderUnit::CacheObject(AEObject * obj)
{
	int result=AE_OK;
	switch(obj->type)
	{
	case AE_OBJ_MESH:
		result&=this->CacheMesh(((AEObjectMesh*)obj)->mesh);
		break;
	}

	for(size_t q=0;q<obj->children.size();q++)
	{
		this->CacheObject(obj->children[q]);
	}

	return result;
}

int AEGLRenderUnit::CacheTexture(AETexture * tex)
{
	int result=AE_OK;

	if(!tex||tex->size<0)
		result=AE_ERR;

	glGenTextures(1,&tex->id);
	this->textures.push_back(tex->id);
	glBindTexture(GL_TEXTURE_2D,tex->id);

	GLenum format=GL_RGB;
	switch(tex->bpp)
	{
	case 24:	format=GL_RGB;	break;
	case 32:	format=GL_RGBA;	break;
	}

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_GENERATE_MIPMAP,GL_TRUE);

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,tex->width,tex->height,0,format,GL_UNSIGNED_BYTE,tex->data);

	if(glGetError()!=GL_NO_ERROR)
		result=AE_ERR;

	return result;
}

int AEGLRenderUnit::CacheMaterials(void)
{
	int result=AE_OK;

	for(unsigned int q=0;q<this->scene->materials.Count();q++)
	{
		result=CacheTexture(this->scene->materials[q]->texture);
		if(result!=AE_OK)
			break;
	}

	return result;
}

int AEGLRenderUnit::CacheFonts(void)
{
	int result=AE_OK;

	for(std::pair<const int,AEFontBitmap> &font:this->scene->fonts.fonts)
	{
		result=CacheTexture(font.second.texture);
		if(result!=AE_OK)
			break;
	}

	return result;
}

int AEGLRenderUnit::CacheMesh(AEMesh * mesh)
{
	if(mesh==NULL) return AE_FALSE;
	if(mesh->cached) return AE_TRUE;

	// //vertices
	// if(mesh->vtxcount>0)
	// {
	// 	unsigned int buf;
	// 	glGenBuffers(1,&buf);
	// 	this->buffers.push_back(buf);
	// 	mesh->idvtx=buf;
	// 	glBindBuffer(GL_ARRAY_BUFFER,buf);
	// 	glBufferData(GL_ARRAY_BUFFER,sizeof(AEVertexf)*mesh->vtxcount,mesh->vtx,GL_STATIC_DRAW);
	// 	glBindBuffer(GL_ARRAY_BUFFER,0);	//Unbound buffers

	// 	if(!CheckError())
	// 		return AE_ERR;
	// }

	// //faces
	// if(mesh->fcecount>0)
	// {
	// 	unsigned int buf;
	// 	glGenBuffers(1,&buf);
	// 	this->buffers.push_back(buf);
	// 	mesh->idfce=buf;
	// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buf);
	// 	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(AETriangle)*mesh->fcecount,mesh->fce,GL_STATIC_DRAW);
	// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);	//Unbound buffers

	// 	if(!CheckError())
	// 		return AE_ERR;
	// }

	// //texture coordinates
	// if(mesh->tcrcount>0)
	// {
	// 	unsigned int buf;
	// 	glGenBuffers(1,&buf);
	// 	this->buffers.push_back(buf);
	// 	mesh->idtcr=buf;
	// 	glBindBuffer(GL_ARRAY_BUFFER,buf);
	// 	glBufferData(GL_ARRAY_BUFFER,sizeof(AETexCoord)*mesh->tcrcount,mesh->tcr,GL_STATIC_DRAW);
	// 	glBindBuffer(GL_ARRAY_BUFFER,0);	//Unbound buffers

	// 	if(!CheckError())
	// 		return AE_ERR;
	// }

	// //normals
	// if(mesh->nrmcount>0)
	// {
	// 	unsigned int buf;
	// 	glGenBuffers(1,&buf);
	// 	this->buffers.push_back(buf);
	// 	mesh->idnrm=buf;
	// 	glBindBuffer(GL_ARRAY_BUFFER,buf);
	// 	glBufferData(GL_ARRAY_BUFFER,sizeof(AEVector3f)*mesh->nrmcount,mesh->nrm,GL_STATIC_DRAW);
	// 	glBindBuffer(GL_ARRAY_BUFFER,0);	//Unbound buffers

	// 	if(!CheckError())
	// 		return AE_ERR;
	// }

	mesh->cached=true;

	mesh->Invalidate(AE_UPDATE_ALL);

	GenerateMeshBuffers(mesh);

	return UpdateMeshBuffers(mesh);
}

void AEGLRenderUnit::GenerateMeshBuffers(AEMesh * mesh)
{
	unsigned int buf[4];
	glGenBuffers(4,buf);

	this->buffers.push_back(buf[0]);
	this->buffers.push_back(buf[1]);
	this->buffers.push_back(buf[2]);
	this->buffers.push_back(buf[3]);

	mesh->idvtx=buf[0];
	mesh->idfce=buf[1];
	mesh->idtcr=buf[2];
	mesh->idnrm=buf[3];
}

int AEGLRenderUnit::UpdateMeshBuffers(AEMesh * mesh)
{
	if(!mesh->cached)
	{
		GenerateMeshBuffers(mesh);
		mesh->cached = true;
	}

	unsigned int mask = mesh->IsInvalid();

	if(mask&AE_UPDATE_VERTEX&&mesh->vtxcount)
	{
		glBindBuffer(GL_ARRAY_BUFFER,mesh->idvtx);
		glBufferData(GL_ARRAY_BUFFER,sizeof(AEVertexf)*mesh->vtxcount,mesh->vtx,GL_STATIC_DRAW);

		if(!CheckError())
			return AE_ERR;

		mesh->Validate(AE_UPDATE_VERTEX);
	}

	if(mask&AE_UPDATE_TEXCOORD&&mesh->tcrcount)
	{
		glBindBuffer(GL_ARRAY_BUFFER,mesh->idtcr);
		glBufferData(GL_ARRAY_BUFFER,sizeof(AETexCoord)*mesh->tcrcount,mesh->tcr,GL_STATIC_DRAW);

		if(!CheckError())
			return AE_ERR;

		mesh->Validate(AE_UPDATE_TEXCOORD);
	}

	if(mask&AE_UPDATE_NORMAL&&mesh->nrmcount)
	{
		glBindBuffer(GL_ARRAY_BUFFER,mesh->idnrm);
		glBufferData(GL_ARRAY_BUFFER,sizeof(AEVector3f)*mesh->nrmcount,mesh->nrm,GL_STATIC_DRAW);

		if(!CheckError())
			return AE_ERR;

		mesh->Validate(AE_UPDATE_NORMAL);
	}

	if(mask&AE_UPDATE_INDEX&&mesh->indcount)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mesh->idfce);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(AETriangle)*mesh->fcecount,mesh->fce,GL_STATIC_DRAW);

		if(!CheckError())
			return AE_ERR;

		mesh->Validate(AE_UPDATE_INDEX);
	}

	//glBindBuffer(GL_ARRAY_BUFFER,0);
	return AE_OK;
}