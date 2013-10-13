/*
 * AEObject.cpp
 *
 *  Created on: Aug 19, 2011
 *      Author: klokik
 */

#include <stddef.h>
#include <math.h>
#include <sstream>

#include "AEObject.h"
#include "AEVectorMath.h"


namespace aengine
{
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

		this->InvalidateTransform();

		this->_bounding_sphere.radius=1.0f;
		this->_bounding_sphere.color={1.0f,1.0f,1.0f,1.0f};

		static int counter=0;
		std::stringstream sstr;
		sstr<<"unnamed_"<<counter++;
		this->name=sstr.str();
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

		InvalidateTransform();
	}

	void AEObject::RelTranslate(AEVector3f vec)
	{
		this->_translate=this->translate+vec;

		InvalidateTransform();
	}

	void AEObject::SetTranslate(AEVector3f vec)
	{
		this->_translate=vec;

		InvalidateTransform();
	}

	void AEObject::RelRotate(AEVector3f vec)
	{
		this->_rotate=this->rotate+vec;

		InvalidateTransform();
	}

	void AEObject::SetRotate(AEVector3f vec)
	{
		this->_rotate=vec;

		InvalidateTransform();
	}

	void AEObject::RelScale(AEVector3f vec)
	{
		this->_scale=this->_scale*vec;

		InvalidateTransform();
	}

	void AEObject::SetScale(AEVector3f vec)
	{
		this->_scale=vec;

		InvalidateTransform();
	}

	void AEObject::InvalidateTransform(void)
	{
		mn_recalc=true;
	}

	void AEObject::CalculateMatrix(bool force)
	{
		if(mn_recalc||force)
		{
			_matrix=AEMatrix4f4().
				Translate(this->translate).
				RotateZ(this->rotate.Z).
				RotateY(this->rotate.Y).
				RotateX(this->rotate.X).
				Scale(this->scale);

			if(this->parent)
				_wmatrix=this->parent->_wmatrix*_matrix;
			else
				_wmatrix=_matrix;

			mn_recalc=false;

			for(AEObject *child:this->_children)
				child->InvalidateTransform();
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

	AEVector3f AEObject::GetAbsPosition(void)
	{
		this->CalculateMatrix();
		AEVector3f vec={_wmatrix[12],_wmatrix[13],_wmatrix[14]};

		return vec;
	}

	AEVector3f AEObject::GetAbsScale(void)
	{
		this->CalculateMatrix();
		AEVector3f vec={_wmatrix[0],_wmatrix[5],_wmatrix[10]};

		return vec;
	}

	AEObject::~AEObject(void)
	{
	}
}
