/*
 * AEMatrix4f4.h
 *
 *  Created on: Jun 18, 2012
 *      Author: klokik
 */

#ifndef AEMATRIX4F4_H_
#define AEMATRIX4F4_H_

#include <stddef.h>

#include "AEUnits.h"

class AEMatrix4f4
{
protected:
	float ret_val[16];		//array which reference would be returned after multiplication and other operations
	float data[16];

public:
	AEMatrix4f4(void);
	AEMatrix4f4(AEMatrix4f4 const &val);
	AEMatrix4f4(float const val[16]);

	void SetIdentity(void);

	float &operator()(const size_t col,const size_t row);
	float &operator[](const size_t id);

	AEMatrix4f4 &operator=(AEMatrix4f4 const &val);
	AEMatrix4f4 &operator=(float const val[16]);

	int operator==(AEMatrix4f4 val);

	float *const operator*(float val);
	float *const operator*(AEMatrix4f4 const &val);
	float *const operator*(float const val[16]);
	AEVector4f operator*(AEVector4f const &vec) const;

	float const *ToArray(void) const;
	operator const float*() const;

	void RotateX(float value);
	void RotateY(float value);
	void RotateZ(float value);
	void RotateXYZ(AEVector3f const &vec);
	void RotateZYX(AEVector3f const &vec);

	void Translate(float x,float y,float z);
	void Translate(AEVector3f const &vec);
	//void TranslateWorld(float x,float y,float z);

	void Scale(float x,float y,float z);
	void Scale(AEVector3f const &vec);

	void Transpose(void);

	virtual ~AEMatrix4f4(void);
};

#endif /* AEMATRIX4F4_H_ */
