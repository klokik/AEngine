/*
 * AEMesh.cpp
 *
 *  Created on: Feb 25, 2012
 *      Author: klokik
 */

#include <stdlib.h>
#include <string.h>

#include "AEMesh.h"

AEMesh::AEMesh(void)
{
	this->vtxcount=0;
	this->fcecount=0;
	this->tcrcount=0;
	this->nrmcount=0;

	this->idvtx=0;
	this->idfce=0;
	this->idtcr=0;
	this->idnrm=0;

	this->vtx=NULL;
	this->nrm=NULL;
	this->tcr=NULL;
	this->fce=NULL;

	this->cached=false;
	this->invalid=false;
}

void AEMesh::Invalidate(unsigned int mask)
{
	this->invalid|=mask;
}

void AEMesh::Validate(unsigned int mask)
{
	this->invalid&=~mask;
}

unsigned int AEMesh::IsInvalid(void)
{
	return this->invalid;
}

AEMesh AEMesh::Clone(void)
{
	throw 0; //not implemented
}

void AEMesh::CloneFrom(AEMesh &mesh)
{

	if(mesh.vtxcount!=vtxcount||
		mesh.fcecount!=fcecount||
		mesh.tcrcount!=tcrcount||
		mesh.nrmcount!=nrmcount)
	{
		this->Free();
		vtx = (AEVertexf*)malloc(sizeof(AEVertexf)*mesh.vtxcount);
		fce = (AETriangle*)malloc(sizeof(AETriangle)*mesh.fcecount);
		tcr = (AETexCoord*)malloc(sizeof(AETexCoord)*mesh.tcrcount);
		nrm = (AEVector3f*)malloc(sizeof(AEVector3f)*mesh.nrmcount);

		vtxcount = mesh.vtxcount;
		fcecount = mesh.fcecount;
		tcrcount = mesh.tcrcount;
		nrmcount = mesh.tcrcount;
	}

	memcpy(vtx,mesh.vtx,sizeof(AEVertexf)*vtxcount);
	memcpy(fce,mesh.fce,sizeof(AETriangle)*fcecount);
	memcpy(tcr,mesh.tcr,sizeof(AETexCoord)*tcrcount);
	memcpy(nrm,mesh.nrm,sizeof(AEVector3f)*nrmcount);
}

AEMesh &AEMesh::operator = (AEMesh &mesh)
{
	CloneFrom(mesh);
	return *this;
}

void AEMesh::Free(void)
{
	if(vtxcount>0&&this->vtx!=NULL)
		free(this->vtx);
	vtxcount=0;
	if(fcecount>0&&this->fce!=NULL)
		free(this->fce);
	fcecount=0;
	if(tcrcount>0&&this->tcr!=NULL)
		free(this->tcr);
	tcrcount=0;
	if(nrmcount>0&&this->nrm!=NULL)
		free(this->nrm);
	nrmcount=0;
}

AEMesh::~AEMesh(void)
{
	this->Free();
}

