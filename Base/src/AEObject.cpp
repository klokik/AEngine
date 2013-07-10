/*
 * AEObject.cpp
 *
 *  Created on: Aug 19, 2011
 *      Author: klokik
 */

#include <stddef.h>
#include <math.h>

#include "AEObject.h"
#include "AEVectorMath.h"


AEObject::AEObject(void):
		type(_type),
		translate(_translate),
		rotate(_rotate),
		scale(_scale),
		children(_children),
		parent(_parent),
		bounding_sphere(_bounding_sphere)
{
	this->_parent=NULL;

	this->SetTranslate(vec3f(0.0f,0.0f,0.0f));
	this->SetRotate(vec3f(0.0f,0.0f,0.0f));
	this->SetScale(vec3f(1.0f,1.0f,1.0f));

	this->visible=true;
	this->projection=AE_PERSPECTIVE;
	this->_type=AE_OBJ_GENERIC;

	this->mn_recalc =true;
	this->tmn_recalc=true;
	this->rmn_recalc=true;
	this->smn_recalc=true;

	this->ctmn_recalc=true;
	this->crmn_recalc=true;
	this->csmn_recalc=true;

	this->_bounding_sphere.radius=1.0f;
	this->_bounding_sphere.color={1.0f,1.0f,1.0f,1.0f};
}

void AEObject::AddChild(AEObject *child)
{
	child->_parent=this;
	this->_children.push_back(child);
}

void AEObject::RemoveChild(AEObject *child)
{
	std::vector<AEObject*>::iterator b=this->_children.begin();
	std::vector<AEObject*>::iterator e=this->_children.end();
	do
	{
		if(*b==child)
		{
			child->_parent=NULL;
			//FIXME: we must either destroy this object or move to scene
			throw 0;
			this->_children.erase(b);
			break;
		}
	}while(b++!=e);
}

void AEObject::Move(AEVector3f vec)
{
	AEVector4f trans=this->GetMatrix()*vec4f(vec,0);
	this->RelTranslate(vec3f(trans.X,trans.Y,trans.Z));

	InvalidateTranslate();
}

void AEObject::RelTranslate(AEVector3f vec)
{
	this->_translate.X+=vec.X;
	this->_translate.Y+=vec.Y;
	this->_translate.Z+=vec.Z;

	InvalidateTranslate();
}

void AEObject::SetTranslate(AEVector3f vec)
{
	this->_translate.X=vec.X;
	this->_translate.Y=vec.Y;
	this->_translate.Z=vec.Z;

	InvalidateTranslate();
}

void AEObject::RelRotate(AEVector3f vec)
{
	this->_rotate.X+=vec.X;
	this->_rotate.Y+=vec.Y;
	this->_rotate.Z+=vec.Z;

	InvalidateRotate();
}

void AEObject::SetRotate(AEVector3f vec)
{
	this->_rotate.X=vec.X;
	this->_rotate.Y=vec.Y;
	this->_rotate.Z=vec.Z;

	InvalidateRotate();
}

void AEObject::RelScale(AEVector3f vec)
{
	this->_scale.X*=vec.X;
	this->_scale.Y*=vec.Y;
	this->_scale.Z*=vec.Z;

	InvalidateScale();
}

void AEObject::SetScale(AEVector3f vec)
{
	this->_scale.X=vec.X;
	this->_scale.Y=vec.Y;
	this->_scale.Z=vec.Z;

	InvalidateScale();
}

void AEObject::InvalidateTranslate(void)
{
	tmn_recalc=true;
	ctmn_recalc=true;
	mn_recalc=true;
}

void AEObject::InvalidateRotate(void)
{
	rmn_recalc=true;
	crmn_recalc=true;
	mn_recalc=true;
}

void AEObject::InvalidateScale(void)
{
	smn_recalc=true;
	csmn_recalc=true;
	mn_recalc=true;
}

void AEObject::InvalidateTransform(void)
{
	mn_recalc=true;
}

void AEObject::CalculateMatrix(bool force)
{
	if(mn_recalc||force)
	{
		//calculate translate rotate and scale of object and w2c matrices
		if(tmn_recalc||force)
		{
			tmatrix.SetIdentity();
			tmatrix.Translate(this->translate);
			tmn_recalc=false;
		}

		if(rmn_recalc||force)
		{
			//TODO: switch way to calculate rotation from object properties
			rmatrix.SetIdentity();
			rmatrix.RotateZYX(this->rotate);
			rmn_recalc=false;
		}

		if(smn_recalc||force)
		{
			smatrix.SetIdentity();
			smatrix.Scale(this->scale);
			smn_recalc=false;
		}

		_matrix=tmatrix*rmatrix;
		_matrix=_matrix*smatrix;

		if(this->parent)
			_wmatrix=this->parent->_wmatrix*_matrix;
		else
			_wmatrix=_matrix;

		mn_recalc=false;

		for(size_t q=0;q<this->_children.size();q++)
		{
			this->_children[q]->mn_recalc=true;
			this->_children[q]->CalculateMatrix(force);
		}
	}
}

void AEObject::CalculateCameraMatrix(bool force)
{
	if(mn_recalc)
	{
		if(ctmn_recalc)
		{
			ctmatrix.SetIdentity();
			ctmatrix.Translate(-translate);
			ctmn_recalc=false;
		}

		if(crmn_recalc)
		{
			crmatrix.SetIdentity();
			crmatrix.RotateXYZ(-rotate);
			crmn_recalc=false;
		}

		if(csmn_recalc)
		{
			csmatrix.SetIdentity();
			csmatrix.Scale(1/scale.X,1/scale.Y,1/scale.Z);
			csmn_recalc=false;
		}

		_wcmatrix=csmatrix*crmatrix;
		_wcmatrix=_wcmatrix*ctmatrix;
		if(this->parent)
			_wcmatrix=_wcmatrix*this->parent->_wcmatrix;
	}
}

const AEMatrix4f4 &AEObject::GetMatrix(void)
{
	this->CalculateMatrix();
	return this->_matrix;
}

const AEMatrix4f4 &AEObject::GetWorldMatrix(void)
{
	this->CalculateMatrix();
	return this->_wmatrix;
}

const AEMatrix4f4 &AEObject::GetCameraMatrix(void)
{
	this->CalculateCameraMatrix();
	return this->_wcmatrix;
}

void AEObject::CalculateBoundingSphere(void)
{
	float max_rad=1.0f;

	AEVector3f trans=this->GetAbsPosition();

	for(AEObject *&child:this->_children)
	{
		child->CalculateBoundingSphere();
		float cr=SqrLength(trans-child->GetAbsPosition())+child->bounding_sphere.radius;
		if(cr>max_rad) max_rad=cr;
	}

	this->_bounding_sphere.radius=sqrt(max_rad);
}

const AEVector3f AEObject::GetAbsPosition(void)
{
	if(this->mn_recalc) this->CalculateMatrix();
	AEVector3f vec={_wmatrix[12],_wmatrix[13],_wmatrix[14]};

	return vec;
}

AEObject::~AEObject(void)
{
}
