/*
*	AEObjectJoint mudule
*	Created on: Mar 1, 2013
*		Author: klokik
*
*	Declares some basic skeletal mechanics
*
*/

#include "AEObjectJoint.h"
#include "AEVectorMath.h"

using namespace aengine;

AEObjectJoint::AEObjectJoint(void):
	tail(_tail)
{
	this->_type=AE_OBJ_JOINT;
	
	this->display_color = {1.0f,1.0f,1.0f,1.0f};

	SetTail(vec3f(0.0f,1.0f,0.0f));
}


void AEObjectJoint::SetTail(AEVector3f vec)
{
	this->_tail = vec;
	this->InvalidateTransform();
}

void AEObjectJoint::SetDisplayColor(AEColor color)
{
	this->display_color = color;
}

// This function is the same as for generic AEObject
// except translate matrix calculation
void AEObjectJoint::CalculateMatrix(bool force)
{
	AEObject::CalculateMatrix(force); //FIXME: Joint matrix
	// if(mn_recalc||force)
	// {
	// 	//calculate translate rotate and scale of object and w2c matrices
	// 	if(tmn_recalc||force)
	// 	{
	// 		tmatrix.SetIdentity();
	// 		if(this->parent&&this->parent->type==AE_OBJ_JOINT)
	// 			tmatrix.Translate(dynamic_cast<AEObjectJoint*>(this->parent)->tail);
	// 		tmatrix.Translate(this->translate);
	// 		tmn_recalc=false;
	// 	}

	// 	if(rmn_recalc||force)
	// 	{
	// 		rmatrix.SetIdentity();
	// 		rmatrix.RotateZ(this->rotate.Z).RotateY(this->rotate.Y).RotateX(this->rotate.X);
	// 		rmn_recalc=false;
	// 	}

	// 	if(smn_recalc||force)
	// 	{
	// 		smatrix.SetIdentity();
	// 		smatrix.Scale(this->scale);
	// 		smn_recalc=false;
	// 	}

	// 	_matrix=tmatrix*rmatrix;
	// 	_matrix=_matrix*smatrix;

	// 	if(this->parent)
	// 		_wmatrix=this->parent->GetWorldMatrix()*_matrix;
	// 	else
	// 		_wmatrix=_matrix;

	// 	mn_recalc=false;

	// 	for(size_t q=0;q<this->_children.size();q++)
	// 	{
	// 		this->_children[q]->InvalidateTransform();
	// 		this->_children[q]->CalculateMatrix(force);
	// 	}
	// }
}

void AEObjectJoint::ApplyDeformation(void)
{
	AEJointDeformAlgorithm algo(this);
	AEMeshDeformer::ApplyDeformation(algo);
}

AEObjectJoint::~AEObjectJoint(void)
{
}