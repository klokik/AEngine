/*
*	AEMeshDeformer mudule
*	Created on: Mar 1, 2013
*		Author: klokik
*
*	Implements some basic skin deformer
*
*/

#include <algorithm>

#include "AEMeshDeformer.h"
#include "AEVectorMath.h"
#include "AEObjectJoint.h"
#include "AEObjectMesh.h"

using namespace aengine;

AEVertexGroup::AEVertexGroup(void)
{
	this->object=NULL;
}

AEVertexGroup::AEVertexGroup(AEObjectMesh *object)
{
	this->object=object;	
}

void AEVertexGroup::AddVertex(size_t id,float weight)
{
	this->items.push_back(std::make_pair(id,weight));
}

bool AEVertexGroup::Validate(void)
{
	if(!(object->mesh_orig&&object->mesh))
		return false;

	for(size_t q=0;q<items.size();q++)
	{
		if(items[q].first>=object->mesh_orig->vtxcount||items[q].first>=object->mesh->vtxcount)
			return false;
	}

	return true;
}

AEVertexGroup::~AEVertexGroup(void)
{
	//Nothing to do here :)
}

AEMeshDeformer::AEMeshDeformer(void)
{
	//nothing to do here
}

void AEMeshDeformer::ApplyDeformation(AEMeshDeformAlgorithm &deformer)
{
	for(auto iter=vertex_groups.begin();iter!=vertex_groups.end();++iter)
		deformer(*iter);
}

AEMeshDeformer::~AEMeshDeformer(void)
{
	//one more stupid function to be cutted off by compiler
}

AEJointDeformAlgorithm::AEJointDeformAlgorithm(void)
{
	AEJointDeformAlgorithm(NULL);
}

AEJointDeformAlgorithm::AEJointDeformAlgorithm(AEObjectJoint *joint)
{
	SetJoint(joint);
}

void AEJointDeformAlgorithm::SetJoint(AEObjectJoint *joint)
{
	this->joint = joint;
}

void AEJointDeformAlgorithm::operator()(AEVertexGroup &vertex_group)
{
	// TODO: find a way to throw named exceptions
	if(!joint)
		throw 1; // joint not specified

	for(auto item=vertex_group.items.begin();item!=vertex_group.items.end();++item)
	{
		AEVector3f vertex = vertex_group.object->mesh_orig->vtx[item->first];

		//FIXME: Normals also need to be recalculated
		//FIXME: Joint position must be used as start point for vertex position
		AEVector4f transformed = joint->GetWorldMatrix()*vec4f(vertex,1.0f);
		vertex = vec3f(transformed.X,transformed.Y,transformed.Z);

		vertex_group.object->mesh->vtx[item->first] = vertex;
	}

	vertex_group.object->mesh->Invalidate(AE_UPDATE_VERTEX);
	// vertex_group.mesh->Invalidate(AE_UPDATE_NORMAL);
}