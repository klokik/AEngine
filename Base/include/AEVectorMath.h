/*
 * AEVectorMath.h
 *
 *  Created on: Jul 6, 2012
 *      Author: klokik
 */

#ifndef AEVECTORMATH_H_
#define AEVECTORMATH_H_

#include <math.h>

#include "AEUnits.h"

const float rad=57.2957795f;

AEVector2f vec2f(float x,float y);
AEVector3f vec3f(float x,float y,float z);
AEVector4f vec4f(float x,float y,float z,float w);

AEVector3f vec3f(AEVector2f v2,float z);
AEVector4f vec4f(AEVector2f v2,float z,float w);
AEVector4f vec4f(AEVector2f v2_1,AEVector2f v2_2);
AEVector4f vec4f(AEVector3f v3,float w);

float Length(AEVector2f vec);
float Length(AEVector3f vec);
float Length(AEVector4f vec);

float SqrLength(AEVector2f vec);
float SqrLength(AEVector3f vec);
float SqrLength(AEVector4f vec);

AEVector2f operator+(const AEVector2f &a,const AEVector2f &b);
AEVector3f operator+(const AEVector3f &a,const AEVector3f &b);
AEVector4f operator+(const AEVector4f &a,const AEVector4f &b);

AEVector2f operator-(const AEVector2f &a,const AEVector2f &b);
AEVector3f operator-(const AEVector3f &a,const AEVector3f &b);
AEVector4f operator-(const AEVector4f &a,const AEVector4f &b);

AEVector2f operator*(const AEVector2f &a,const float b);
AEVector3f operator*(const AEVector3f &a,const float b);
AEVector4f operator*(const AEVector4f &a,const float b);

AEVector2f operator/(const AEVector2f &a,const float b);
AEVector3f operator/(const AEVector3f &a,const float b);
AEVector4f operator/(const AEVector4f &a,const float b);

float dot(const AEVector2f &a,const AEVector2f &b);
float dot(const AEVector3f &a,const AEVector3f &b);
float dot(const AEVector4f &a,const AEVector4f &b);

AEVector3f cross(const AEVector3f &a,const AEVector3f &b);

AEVector2f operator-(const AEVector2f &a);
AEVector3f operator-(const AEVector3f &a);
AEVector4f operator-(const AEVector4f &a);

template <typename T>
T normalize(const T &a)
{
	return a/Length(a);
}

#endif /* AEVECTORMATH_H_ */
