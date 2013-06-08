/*
 * AEObjectMesh.h
 *
 *  Created on: Aug 21, 2011
 *      Author: klokik
 */

#ifndef AEOBJECTMESH_H_
#define AEOBJECTMESH_H_

#include "AEObject.h"
#include "AEMesh.h"
#include "AEMaterial.h"

namespace aengine
{
	class AEObjectMesh: public AEObject
	{
	public:
		AEMesh *mesh;	// transformed
		AEMesh *mesh_orig;

		AEMaterial *material;

		AEObjectMesh();

		virtual void CalculateBoundingSphere(void);

		void SetMesh(AEMesh *mesh);
		void RestoreMesh(void);
		void BakeMesh(bool copy=true);

		virtual ~AEObjectMesh();
	};
}

#endif /* AEOBJECTMESH_H_ */
