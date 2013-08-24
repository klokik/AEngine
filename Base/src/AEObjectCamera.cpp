/*
 * AEObjExtCamera.cpp
 *
 *  Created on: Sep 17, 2011
 *      Author: klokik
 */

#include "AEObjectCamera.h"

#include "AEVectorMath.h"

AEObjectCamera::AEObjectCamera(void)
{
	this->_type=AE_OBJ_CAMERA;

	this->angle=45.0f;
	this->focus=0.0f;
	this->z_near=0.1f;
	this->z_far=100.0f;

	this->InvalidateProjection();
}

//force means that complete path from root element will be recalculated (not implemented yet)
void AEObjectCamera::CalculateCameraMatrix(bool force)
{
	AEObject *rparent=this;
	while(rparent)
	{
		root_path.push(rparent);
		if(rparent->type==AE_OBJ_CAMERA&&rparent!=this)
			break;
		rparent=rparent->parent;
	}

	//TODO implement non-force way
	do
	{
		AEObject *node=root_path.top();
		//TODO !!! it's commented for test (does i need it???) !!! node->mn_recalc=true;
		if(node==this)
			AEObject::CalculateCameraMatrix(force);
		else
			node->CalculateCameraMatrix(force);
		root_path.pop();
	}while(!root_path.empty());
}

void AEObjectCamera::CalculateProjectionMatrix(bool force)
{
	if(pmn_recalc||force)
	{
		float r=z_near/tan(angle/2);
		float t=r/ratio;

		float p_mtx[16]={
			z_near/r, 0,0,0,
			0, z_near/t, 0,0,
			0,0,-(z_far+z_near)/(z_far-z_near),-1,
			0,0,-2*z_far*z_near/(z_far-z_near),0
		};
		pmatrix=p_mtx;

		pmn_recalc=false;
	}
}

const AEMatrix4f4 &AEObjectCamera::GetProjectionMatrix(void)
{
	this->CalculateProjectionMatrix();
	return this->pmatrix;
}

void AEObjectCamera::InvalidateProjection(void)
{
	pmn_recalc=true;
}

void AEObjectCamera::LookAt(AEVector3f point,AEVector3f normal)
{
	this->SetLookDirection(point-GetAbsPosition(),normal);
}

void AEObjectCamera::SetLookDirection(AEVector3f vector,AEVector3f normal)
{
	//TODO: We need to consider case, if this camera is child of some other object
	// for now I use only matrix and don't change coordinates

	//set new basis as normalized vectors of new direction
	//AEVector3f e_x = cross(normal,vector);
	//now we have our matrix and we need to set it (e_x,normal,vector)
}

AEObjectCamera::~AEObjectCamera(void)
{
}

