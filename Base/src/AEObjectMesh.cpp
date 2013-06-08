/*
 * AEObjectMesh.cpp
 *
 *  Created on: Aug 21, 2011
 *      Author: klokik
 */

#include <stddef.h>

#include "AEObjectMesh.h"


using namespace aengine;

AEObjectMesh::AEObjectMesh()
{
	this->_type=AE_OBJ_MESH;
	this->mesh=NULL;
	this->mesh_orig=NULL;
	this->material=NULL;
}

void AEObjectMesh::CalculateBoundingSphere(void)
{
	AEObject::CalculateBoundingSphere();

	if(this->material)
		this->_bounding_sphere.color=this->material->diffuse;
}

void AEObjectMesh::SetMesh(AEMesh *mesh)
{
	if(mesh_orig)
		delete mesh_orig;

	mesh_orig = mesh;

	RestoreMesh();
}

void AEObjectMesh::RestoreMesh(void)
{
	if(!mesh)
		mesh = new AEMesh;
	mesh->CloneFrom(*mesh_orig);

	mesh->Invalidate(AE_UPDATE_ALL);
}

void AEObjectMesh::BakeMesh(bool copy)
{
	if(!copy)
		mesh_orig = mesh;
	else
	{
		if(mesh_orig&&mesh!=mesh_orig)
			delete mesh_orig;
		mesh_orig = new AEMesh;
		mesh_orig->CloneFrom(*mesh);
	}
}

AEObjectMesh::~AEObjectMesh()
{
	//TODO store mesh in some container, and free memory correctly
	if(mesh!=NULL)
		delete this->mesh;
	if(mesh_orig!=NULL)
		delete mesh_orig;
}

