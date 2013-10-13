/*
 * AEObject.h
 *
 *  Created on: Aug 19, 2011
 *      Author: klokik
 */

#ifndef AEOBJECT_H_
#define AEOBJECT_H_

#include <vector>
#include <map>
#include <string>

#include "AEDefines.h"
#include "AEUnits.h"
#include "AEMatrix4f4.h"
#include "AEBoundingSphere.h"


namespace aengine
{
	class AEObject
	{
	protected:
		AEObject *_parent;

		std::vector<AEObject*> _children;
		AE_OBJ_TYPE _type;

		AEVector3f _translate;
		AEVector3f _rotate;
		AEVector3f _scale;

		AEMatrix4f4 _matrix;	//object matrix
		AEMatrix4f4 _wmatrix;	//world matrix

		bool mn_recalc;

		AEBoundingSphere _bounding_sphere;

	public:
		std::string name;
		
		int projection;	//either AE_ORTHOGRAPHIC or AE_PERSPECTIVE

		bool visible;

		const AE_OBJ_TYPE &type;

		const AEVector3f &translate;
		const AEVector3f &rotate;
		const AEVector3f &scale;

		const std::vector<AEObject*> &children;
		AEObject * const &parent;

		const AEBoundingSphere &bounding_sphere;

		AEObject(void);

		void AddChild(AEObject* child);
		void RemoveChild(AEObject* child);

		virtual void Move(AEVector3f vec);

		void RelTranslate(AEVector3f vec);
		void SetTranslate(AEVector3f vec);

		void RelRotate(AEVector3f vec);
		void SetRotate(AEVector3f vec);

		void RelScale(AEVector3f vec);
		void SetScale(AEVector3f vec);

		virtual void InvalidateTransform(void);

		virtual void CalculateMatrix(bool force=false);

		const AEMatrix4f4 &GetMatrix(void);
		const AEMatrix4f4 &GetWorldMatrix(void);

		//Calculates bounding spheres for each element in branch
		virtual void CalculateBoundingSphere(void);

		AEVector3f GetAbsPosition(void);
		AEVector3f GetAbsScale(void);

		virtual ~AEObject(void);
	};
}

#endif /* AEOBJECT_H_ */
