/*
 * AEObjExtCamera.cpp
 *
 *  Created on: Sep 17, 2011
 *      Author: klokik
 */

#include "AEObjectCamera.h"

#include "AEVectorMath.h"


namespace aengine
{
	AEObjectCamera::AEObjectCamera(void)
	{
		this->_type=AE_OBJ_CAMERA;

		this->angle=120.0f;
		this->focus=0.0f;
		this->z_near=0.1f;
		this->z_far=100.0f;

		this->InvalidateTransform();
		this->InvalidateProjection();
	}

	void AEObjectCamera::InvalidateTransform(void)
	{
		AEObject::InvalidateTransform();

		cmn_recalc=true;
	}

	void AEObjectCamera::CalculateCameraMatrix(bool force)
	{
		if(cmn_recalc)
		{
			CalculateMatrix(force);

			_wcmatrix=_wmatrix.Invert();

			cmn_recalc=false;
		}
	}

	void AEObjectCamera::CalculateProjectionMatrix(bool force)
	{
		float r_angle=angle/57.2957795131f;

		if(pmn_recalc||force)
		{
			float tg=tan(r_angle/2);

			float p_mtx[16]={
				1/tg ,0,0,0,
				0, ratio/tg, 0, 0,
				0,0,-(z_far+z_near)/(z_far-z_near),-1,
				0,0,-2*z_far*z_near/(z_far-z_near),0
			};
			pmatrix=p_mtx;

			pmn_recalc=false;
		}
	}

	const AEMatrix4f4 &AEObjectCamera::GetCameraMatrix(void)
	{
		this->CalculateCameraMatrix();
		return this->_wcmatrix;
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
}
