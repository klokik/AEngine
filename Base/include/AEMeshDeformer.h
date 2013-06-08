/*
*	AEMeshDeformer mudule
*	Created on: Mar 1, 2013
*		Author: klokik
*
*	Declares some basic skin deformer
*
*/

#ifndef AEMESHDEFORMER_H_
#define AEMESHDEFORMER_H_

#include <vector>
#include <stddef.h>

#include "AEMesh.h"

namespace aengine
{
	class AEObjectJoint;
	class AEObjectMesh;

	class AEVertexGroup
	{
	public:
		AEObjectMesh *object;

		std::vector< std::pair<size_t, float> > items; // pairs of indexes and weights

		AEVertexGroup(void);
		AEVertexGroup(AEObjectMesh *object);

		void AddVertex(size_t id,float weight);

		bool Validate(void);

		~AEVertexGroup(void);
	};

	class AEMeshDeformAlgorithm
	{
	public:
		virtual void operator()(AEVertexGroup &vertex_group)=0;
	};

	class  AEMeshDeformer
	{
	public:
		std::vector<AEVertexGroup> vertex_groups;

		AEMeshDeformer(void);

		void ApplyDeformation(AEMeshDeformAlgorithm &deformer);

		virtual ~AEMeshDeformer(void);
	};

	class AEJointDeformAlgorithm: public AEMeshDeformAlgorithm
	{
	public:
		AEObjectJoint *joint;

		AEJointDeformAlgorithm(void);
		AEJointDeformAlgorithm(AEObjectJoint *joint);

		void SetJoint(AEObjectJoint *joint);

		virtual void operator()(AEVertexGroup &vertex_group);
	};
}

#endif /* AEMESHDEFORMER_H_ */