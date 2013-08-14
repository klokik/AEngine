/*
 * AEObjectCamera.h
 *
 *  Created on: Sep 17, 2011
 *      Author: klokik
 */

#ifndef AEOBJECTCAMERA_H_
#define AEOBJECTCAMERA_H_

#include <stack>

#include "AEObject.h"

class AEObjectCamera: public AEObject
{
protected:
	std::stack< AEObject*,std::vector<AEObject*> > root_path; //path to root node of tree

	AEMatrix4f4 pmatrix;  // projection matrix

	bool pmn_recalc; // true, if matrix needs to be recalculated

public:
	float focus;

	// frustrum geometry
	float angle;		// X - axis
	float ratio;		// X to Y
	float z_near; 	// near plane
	float z_far;		// far plane

	AEObjectCamera(void);

	virtual void CalculateCameraMatrix(bool force=false);
	virtual void CalculateProjectionMatrix(bool force=false);

	const AEMatrix4f4 &GetProjectionMatrix(void);

	void InvalidateProjection(void);

	virtual void LookAt(AEVector3f point,AEVector3f normal);
	virtual void SetLookDirection(AEVector3f vector,AEVector3f normal);

	virtual ~AEObjectCamera(void);
};

#endif /* AEOBJECTCAMERA_H_ */
