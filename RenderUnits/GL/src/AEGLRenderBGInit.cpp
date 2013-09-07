/*
 * AEGLRenderBGInit.cpp
 *
 *  Created on: Jun 15, 2012
 *      Author: klokik
 *
 *      This file contains definitions of basic geometry object
 *      like sprites, empty objects, bones, cameras, light sources etc.
 *
 *      For rendering object look AEGLRender[Object type].cpp files
 */

#include <string.h>

#include "AEGLHeader.h"
#include "AEGLRender.h"


namespace aengine
{
	int AEGLRenderUnit::InitBasicGeometry(void)
	{
		int result=AE_OK;

		result&=InitBGEmpty();
		result&=InitBGSprite();
		result&=InitBGLCube();

		return result;
	}

	int AEGLRenderUnit::InitBGEmpty(void)
	{
		empty_mesh.vtxcount=6;
		empty_mesh.nrmcount=6;	//normal buffer used as a color one

		float vtx[]={
				-1.0f, 0.0f, 0.0f,
				 1.0f, 0.0f, 0.0f,
				 0.0f,-1.0f, 0.0f,
				 0.0f, 1.0f, 0.0f,
				 0.0f, 0.0f,-1.0f,
				 0.0f, 0.0f, 1.0f};

		float col[]={
				 0.5f, 0.0f, 0.0f, 1.0f,
				 1.0f, 0.0f, 0.0f, 1.0f,
				 0.0f, 0.5f, 0.0f, 1.0f,
				 0.0f, 1.0f, 0.0f, 1.0f,
				 0.0f, 0.0f, 0.5f, 1.0f,
				 0.0f, 0.0f, 1.0f, 1.0f};

		glGenBuffers(1,&empty_mesh.idvtx);
		glGenBuffers(1,&empty_mesh.idnrm);

		glBindBuffer(GL_ARRAY_BUFFER,empty_mesh.idvtx);
		glBufferData(GL_ARRAY_BUFFER,sizeof(AEVertexf)*6,vtx,GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER,empty_mesh.idnrm);
		glBufferData(GL_ARRAY_BUFFER,sizeof(AEColor)*6,col,GL_STATIC_DRAW);


		return AE_OK;
	}

	int AEGLRenderUnit::InitBGSprite(void)
	{
		sprite_mesh.vtxcount=4;
		sprite_mesh.tcrcount=4;
		sprite_mesh.nrmcount=4;
		sprite_mesh.fcecount=2;

		//        X     Y     Z
		float vtx[]={
				-0.5f, 0.5f, 0.0f,
				 0.5f, 0.5f, 0.0f,
				 0.5f,-0.5f, 0.0f,
				-0.5f,-0.5f, 0.0f };

		float nrm[]={
				 0.0f, 0.0f,-1.0f,
				 0.0f, 0.0f,-1.0f,
				 0.0f, 0.0f,-1.0f,
				 0.0f, 0.0f,-1.0f};
		//        U     V     W
		float tcr[]={
				 0.0f, 0.0f, 0.0f,
				 1.0f, 0.0f, 0.0f,
				 1.0f, 1.0f, 0.0f,
				 0.0f, 1.0f, 0.0f };

		unsigned int fce[]={ 0, 1, 2, 2, 3, 0 };

		sprite_mesh.vtx=(AEVertexf*)malloc(sizeof(vtx));
		sprite_mesh.tcr=(AETexCoord*)malloc(sizeof(tcr));
		sprite_mesh.nrm=(AEVector3f*)malloc(sizeof(nrm));
		sprite_mesh.fce=(AETriangle*)malloc(sizeof(fce));

		memcpy(sprite_mesh.vtx,vtx,sizeof(vtx));
		memcpy(sprite_mesh.tcr,tcr,sizeof(tcr));
		memcpy(sprite_mesh.nrm,nrm,sizeof(nrm));
		memcpy(sprite_mesh.fce,fce,sizeof(fce));

		AEGLRenderUnit::CacheMesh(&sprite_mesh);

		return AE_OK;
	}

	int AEGLRenderUnit::InitBGLCube(void)
	{
		lcube_mesh.vtxcount=8;
		lcube_mesh.indcount=24;

		float vtx[]={
				-1.0f, 1.0f, 1.0f,//front
				 1.0f, 1.0f, 1.0f,
				 1.0f,-1.0f, 1.0f,
				-1.0f,-1.0f, 1.0f,
				-1.0f, 1.0f,-1.0f,//back
				 1.0f, 1.0f,-1.0f,
				 1.0f,-1.0f,-1.0f,
				-1.0f,-1.0f,-1.0f};

		unsigned int ind[]={
				0,1, 1,2, 2,3, 3,0,
				4,5, 5,6, 6,7, 7,4,
				0,4, 1,5, 2,6, 3,7};

		glGenBuffers(1,&lcube_mesh.idvtx);
		glGenBuffers(1,&lcube_mesh.idind);

		glBindBuffer(GL_ARRAY_BUFFER,lcube_mesh.idvtx);
		glBufferData(GL_ARRAY_BUFFER,sizeof(AEVertexf)*8,vtx,GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,lcube_mesh.idind);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)*24,ind,GL_STATIC_DRAW);

		return AE_OK;
	}

	int AEGLRenderUnit::InitBGTetra(void)
	{
		tetra_mesh.vtxcount=4;

		float vtx[]={
				 0.0f, 1.0f, 0.0f,
				 0.5f, 0.0f, 0.0f,
				 0.0f, 1.0f, 0.0f,
				-0.25f,0.0f, 0.43301f,
				 0.0f, 1.0f, 0.0f,
				-0.25f,0.0f,-0.43301f,

				 0.5f, 0.0f, 0.0f,
				 -0.25f,0.0f, 0.43301f,
				 -0.25f,0.0f, 0.43301f,
				 -0.25f,0.0f,-0.43301f,
				 -0.25f,0.0f,-0.43301f,
				 0.5f, 0.0f, 0.0f};


		glGenBuffers(1,&tetra_mesh.idvtx);

		glBindBuffer(GL_ARRAY_BUFFER,tetra_mesh.idvtx);
		glBufferData(GL_ARRAY_BUFFER,sizeof(AEVertexf)*12,vtx,GL_STATIC_DRAW);


		return AE_OK;
	}

	void AEGLRenderUnit::FreeBGBuffers(void)
	{
		//Sprite
		if(glIsBuffer(sprite_mesh.idvtx)) glDeleteBuffers(1,&sprite_mesh.idvtx);
		if(glIsBuffer(sprite_mesh.idnrm)) glDeleteBuffers(1,&sprite_mesh.idnrm);
		if(glIsBuffer(sprite_mesh.idtcr)) glDeleteBuffers(1,&sprite_mesh.idtcr);
		if(glIsBuffer(sprite_mesh.idfce)) glDeleteBuffers(1,&sprite_mesh.idfce);

		//Empty
		if(glIsBuffer(empty_mesh.idvtx)) glDeleteBuffers(1,&empty_mesh.idvtx);
		if(glIsBuffer(empty_mesh.idnrm)) glDeleteBuffers(1,&empty_mesh.idnrm);

		//LCube
		if(glIsBuffer(lcube_mesh.idvtx)) glDeleteBuffers(1,&lcube_mesh.idvtx);
		if(glIsBuffer(lcube_mesh.idfce)) glDeleteBuffers(1,&lcube_mesh.idfce);

		//Tetra
		if(glIsBuffer(tetra_mesh.idvtx)) glDeleteBuffers(1,&tetra_mesh.idvtx);
	}
}
