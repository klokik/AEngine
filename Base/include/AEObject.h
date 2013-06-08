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

class AEObject
{
protected:
	std::string name;

	AEObject *_parent;

	std::vector<AEObject*> _children;
	AE_OBJ_TYPE _type;

	AEVector3f _translate;
	AEVector3f _rotate;
	AEVector3f _scale;

	AEMatrix4f4 tmatrix;	//Translate matrix
	AEMatrix4f4 rmatrix;	//Rotate matrix
	AEMatrix4f4 smatrix;	//Scale matrix

	AEMatrix4f4 ctmatrix;	//Camera translate matrix
	AEMatrix4f4 crmatrix;	//Camera rotate matrix
	AEMatrix4f4 csmatrix;	//Camera scale matrix

	AEMatrix4f4 _matrix;	//smatrix*tmatrix*rmatrix
	AEMatrix4f4 _wmatrix;	//parent->wmatrix*this->wmatrix
	AEMatrix4f4 _wcmatrix;	//world2camera matrix

	bool tmn_recalc;
	bool rmn_recalc;
	bool smn_recalc;

	bool ctmn_recalc;	//Camera matrix recalculations
	bool crmn_recalc;
	bool csmn_recalc;

	bool mn_recalc;

	AEBoundingSphere _bounding_sphere;

public:
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
	void RemChild(AEObject* child);

	virtual void Move(AEVector3f vec);

	void RelTranslate(AEVector3f vec);
	void SetTranslate(AEVector3f vec);

	void RelRotate(AEVector3f vec);
	void SetRotate(AEVector3f vec);

	void RelScale(AEVector3f vec);
	void SetScale(AEVector3f vec);

	void InvalidateTranslate(void);
	void InvalidateRotate(void);
	void InvalidateScale(void);
	void InvalidateTransform(void);

	virtual void CalculateMatrix(bool force=false);
	virtual void CalculateCameraMatrix(bool force=false);

	const AEMatrix4f4 &GetMatrix(void);
	const AEMatrix4f4 &GetWorldMatrix(void);
	const AEMatrix4f4 &GetCameraMatrix(void);

	//Calculates bounding spheres for each element in branch
	virtual void CalculateBoundingSphere(void);

	const AEVector3f GetAbsPosition(void);

	virtual ~AEObject(void);
};

#endif /* AEOBJECT_H_ */
