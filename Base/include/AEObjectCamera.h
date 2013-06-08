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

public:
	float angle;
	float focus;

	AEObjectCamera(void);

	virtual void CalculateCameraMatrix(bool force=false);

	virtual void LookAt(AEVector3f point,AEVector3f normal);
	virtual void SetLookDirection(AEVector3f vector,AEVector3f normal);

	virtual ~AEObjectCamera(void);
};

#endif /* AEOBJECTCAMERA_H_ */
