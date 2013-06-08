/*
*	AEObjectJoint mudule
*	Created on: Mar 1, 2013
*		Author: klokik
*
*	Declares some basic skeletal mechanics
*
*/

#ifndef AEOBJECTJOINT_H_
#define AEOBJECTJOINT_H_

#include "AEObject.h"
#include "AEMeshDeformer.h"

namespace aengine
{
	class AEObjectJoint: public AEObject, public AEMeshDeformer
	{
	protected:
		AEVector3f _tail;

	public:
		AEColor display_color;
		const AEVector3f &tail;

		AEObjectJoint(void);

		void SetTail(AEVector3f vec);
		void SetDisplayColor(AEColor color);

		virtual void CalculateMatrix(bool force=false);

		void ApplyDeformation(void);

		virtual ~AEObjectJoint(void);
	};
}

#endif /* AEOBJECTJOINT_H_ */