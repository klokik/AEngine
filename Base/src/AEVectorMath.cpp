/*
 * AEVectorMath.cpp
 *
 *  Created on: Jul 6, 2012
 *      Author: klokik
 */

#include <sstream>

#include "AEVectorMath.h"


AEVector2f vec2f(float x,float y)
{
	return {x,y};
}

AEVector3f vec3f(float x,float y,float z)
{
	return {x,y,z};
}

AEVector4f vec4f(float x,float y,float z,float w)
{
	return {x,y,z,w};
}

AEVector3f vec3f(AEVector2f v2,float z)
{
	return {v2.X,v2.Y,z};
}

AEVector4f vec4f(AEVector2f v2,float z,float w)
{
	return {v2.X,v2.Y,z,w};
}

AEVector4f vec4f(AEVector2f v2_1,AEVector2f v2_2)
{
	return {v2_1.X,v2_1.Y,v2_2.X,v2_2.Y};
}

AEVector4f vec4f(AEVector3f v3,float w)
{
	return {v3.X,v3.Y,v3.Z,w};
}

float Length(AEVector2f vec)
{
	return sqrtf(SqrLength(vec));
}

float Length(AEVector3f vec)
{
	return sqrtf(SqrLength(vec));
}

float Length(AEVector4f vec)
{
	return sqrtf(SqrLength(vec));
}

float SqrLength(AEVector2f vec)
{
	return vec.X*vec.X+vec.Y*vec.Y;
}

float SqrLength(AEVector3f vec)
{
	return vec.X*vec.X+vec.Y*vec.Y+vec.Z*vec.Z;
}

float SqrLength(AEVector4f vec)
{
	return vec.X*vec.X+vec.Y*vec.Y+vec.Z*vec.Z+vec.W*vec.W;
}

AEVector2f operator +(const AEVector2f &a,const AEVector2f &b)
{
	return {a.X+b.X,a.Y+b.Y};
}

AEVector3f operator +(const AEVector3f &a,const AEVector3f &b)
{
	return {a.X+b.X,a.Y+b.Y,a.Z+b.Z};
}

AEVector4f operator +(const AEVector4f &a,const AEVector4f &b)
{
	return {a.X+b.X,a.Y+b.Y,a.Z+b.Z,a.W+b.W};
}

AEVector2f operator -(const AEVector2f &a,const AEVector2f &b)
{
	return {a.X-b.X,a.Y-b.Y};
}

AEVector3f operator -(const AEVector3f &a,const AEVector3f &b)
{
	return {a.X-b.X,a.Y-b.Y,a.Z-b.Z};
}

AEVector4f operator -(const AEVector4f &a,const AEVector4f &b)
{
	return {a.X-b.X,a.Y-b.Y,a.Z-b.Z,a.W-b.W};
}

AEVector2f operator *(const AEVector2f &a,const float b)
{
	return {a.X*b,a.Y*b};
}

AEVector3f operator *(const AEVector3f &a,const float b)
{
	return {a.X*b,a.Y*b,a.Z*b};
}

AEVector4f operator *(const AEVector4f &a,const float b)
{
	return {a.X*b,a.Y*b,a.Z*b,a.W*b};
}

AEVector2f operator*(const AEVector2f a,const AEVector2f b)
{
	return {a.X*b.X,a.Y*b.Y};
}

AEVector3f operator*(const AEVector3f a,const AEVector3f b)
{
	return {a.X*b.X,a.Y*b.Y,a.Z*b.Z};
}

AEVector4f operator*(const AEVector4f a,const AEVector4f b)
{
	return {a.X*b.X,a.Y*b.Y,a.Z*b.Z,a.W*b.W};
}


AEVector2f operator /(const AEVector2f &a,const float b)
{
	return {a.X/b,a.Y/b};
}

AEVector3f operator /(const AEVector3f &a,const float b)
{
	return {a.X/b,a.Y/b,a.Z/b};
}

AEVector4f operator /(const AEVector4f &a,const float b)
{
	return {a.X/b,a.Y/b,a.Z/b,a.W/b};
}

bool operator==(const AEVector2f a,const AEVector2f b)
{
	return (a.X==b.X)&&(a.Y==b.Y);
}

bool operator==(const AEVector3f a,const AEVector3f b)
{
	return (a.X==b.X)&&(a.Y==b.Y)&&(a.Z==b.Z);
}

bool operator==(const AEVector4f a,const AEVector4f b)
{
	return (a.X==b.X)&&(a.Y==b.Y)&&(a.Z==b.Z)&&(a.W==b.W);
}

bool operator!=(const AEVector2f a,const AEVector2f b)
{
	return !(a==b);
}

bool operator!=(const AEVector3f a,const AEVector3f b)
{
	return !(a==b);
}

bool operator!=(const AEVector4f a,const AEVector4f b)
{
	return !(a==b);
}

float dot(const AEVector2f &a,const AEVector2f &b)
{
	return a.X*b.X+a.Y*b.Y;
}

float dot(const AEVector3f &a,const AEVector3f &b)
{
	return a.X*b.X+a.Y*b.Y+a.Z*b.Z;
}

float dot(const AEVector4f &a,const AEVector4f &b)
{
	return a.X*b.X+a.Y*b.Y+a.Z*b.Z+a.W*b.W;
}

AEVector3f cross(const AEVector3f &a,const AEVector3f &b)
{
	return {a.Y*b.Z-a.Z*b.Y,a.Z*b.X-a.X*b.Z,a.X*b.Y-a.Y*b.Z};
}

AEVector2f operator-(const AEVector2f &a)
{
	return a*(-1.0f);
}

AEVector3f operator-(const AEVector3f &a)
{
	return a*(-1.0f);
}

AEVector4f operator-(const AEVector4f &a)
{
	return a*(-1.0f);
}

std::ostream &operator<<(std::ostream &os,const AEVector2f vec)
{
	std::stringstream sstr;
	sstr<<"("<<vec.X<<","<<vec.Y<<")";

	return (os<<sstr.str());
}

std::ostream &operator<<(std::ostream &os,const AEVector3f vec)
{
	std::stringstream sstr;
	sstr<<"("<<vec.X<<","<<vec.Y<<","<<vec.Z<<")";

	return (os<<sstr.str());
}

std::ostream &operator<<(std::ostream &os,const AEVector4f vec)
{
	std::stringstream sstr;
	sstr<<"("<<vec.X<<","<<vec.Y<<","<<vec.Z<<","<<vec.W<<")";

	return (os<<sstr.str());
}