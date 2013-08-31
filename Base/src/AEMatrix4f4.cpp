/*
 * AEMatrix4f4.cpp
 *
 *  Created on: Jun 18, 2012
 *      Author: klokik
 */

#include "AEMatrix4f4.h"

#include <string.h>
#include <math.h>
#include <algorithm>

#define fdcos(a) (float)cos(a/57.2957795f)
#define fdsin(a) (float)sin(a/57.2957795f)

AEMatrix4f4::AEMatrix4f4(void)
{
	this->SetIdentity();
}

AEMatrix4f4::AEMatrix4f4(AEMatrix4f4 const &val)
{
	this->operator =(val);
}

AEMatrix4f4::AEMatrix4f4(float const val[])
{
	this->operator =(val);
}

void AEMatrix4f4::SetIdentity(void)
{
	memset((void*)this->data,0,sizeof(float)*16);
	this->data[ 0]=1.0f;
	this->data[ 5]=1.0f;
	this->data[10]=1.0f;
	this->data[15]=1.0f;
}

float &AEMatrix4f4::operator ()(const size_t col, const size_t row)
{
	return this->data[col*4+row];
}

float &AEMatrix4f4::operator [](const size_t id)
{
	return this->data[id];
}

AEMatrix4f4 &AEMatrix4f4::operator =(AEMatrix4f4 const &val)
{
	memcpy((void*)this->data,(void*)val.data,sizeof(float)*16);
	return *this;
}

AEMatrix4f4 &AEMatrix4f4::operator =(float const val[16])
{
	memcpy((void*)this->data,(void*)val,sizeof(float)*16);
	return *this;
}

int AEMatrix4f4::operator ==(AEMatrix4f4 val)
{
	return memcmp((const void*)this->data,(const void*)val.data,sizeof(float)*16);
}

float *const AEMatrix4f4::operator *(float val)
{
	for(size_t q=0;q<16;q++)
		ret_val[q]=data[q]*val;

	return this->ret_val;
}

float *const AEMatrix4f4::operator *(AEMatrix4f4 const &val)
{
	return this->operator *(val.data);
}

float *const AEMatrix4f4::operator *(float const val[16])
{
	/*
	 * +----------------+
	 * | 00  04  08  12 |
	 * | 01  05  09  13 |
	 * | 02  06  10  14 |
	 * | 03  07  11  15 |
	 * +----------------+
	 */
	
	const float *a=this->data;
	const float *b=val;
	float *c=this->ret_val;

	for(int q=0;q<16;q+=4)
		for(int w=0;w<4;w++)
			c[q+w]=a[w]*b[q]+a[w+4]*b[q+1]+a[w+8]*b[q+2]+a[w+12]*b[q+3];


	return this->ret_val;
}

AEVector4f AEMatrix4f4::operator *(const AEVector4f& vec) const
{
	AEVector4f res;
	const float *a=this->data;

	res.X=a[0]*vec.X+a[4]*vec.Y+a[ 8]*vec.Z+a[12]*vec.W;
	res.Y=a[1]*vec.X+a[5]*vec.Y+a[ 9]*vec.Z+a[13]*vec.W;
	res.Z=a[2]*vec.X+a[6]*vec.Y+a[10]*vec.Z+a[14]*vec.W;
	res.W=a[3]*vec.X+a[7]*vec.Y+a[11]*vec.Z+a[15]*vec.W;

	return res;
}

float const *AEMatrix4f4::ToArray(void) const
{
	return this->data;
}

AEMatrix4f4::operator const float*() const
{
	return this->ToArray();
}

void AEMatrix4f4::RotateX(float value)
{
	if(value==0.0f) return;
	float mt[]={
			1.0f, 0.0f,         0.0f,         0.0f,
			0.0f, fdcos(value), fdsin(value), 0.0f,
			0.0f,-fdsin(value), fdcos(value), 0.0f,
			0.0f, 0.0f,         0.0f,         1.0f};

	(*this)=this->operator *(mt);
}

void AEMatrix4f4::RotateY(float value)
{
	if(value==0.0f) return;
	float mt[]={
			fdcos(value), 0.0f,-fdsin(value), 0.0f,
			0.0f,         1.0f, 0.0f,         0.0f,
			fdsin(value), 0.0f, fdcos(value), 0.0f,
			0.0f,         0.0f, 0.0f,         1.0f};

	(*this)=this->operator *(mt);
}

void AEMatrix4f4::RotateZ(float value)
{
	if(value==0.0f) return;
	float mt[]={
			fdcos(value), fdsin(value), 0.0f, 0.0f,
			-fdsin(value),fdcos(value), 0.0f, 0.0f,
			0.0f,         0.0f,         1.0f, 0.0f,
			0.0f,         0.0f,         0.0f, 1.0f};

	(*this)=this->operator *(mt);
}

void AEMatrix4f4::RotateXYZ(AEVector3f const &vec)
{
	this->RotateX(vec.X);
	this->RotateY(vec.Y);
	this->RotateZ(vec.Z);
}

void AEMatrix4f4::RotateZYX(AEVector3f const &vec)
{
	this->RotateZ(vec.Z);
	this->RotateY(vec.Y);
	this->RotateX(vec.X);
}

void AEMatrix4f4::Translate(float x, float y, float z)
{
	float mt[]={
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			   x,    y,    z, 1.0f};

	(*this)=this->operator *(mt);
}

void AEMatrix4f4::Translate(const AEVector3f& vec)
{
	this->Translate(vec.X,vec.Y,vec.Z);
}

void AEMatrix4f4::Scale(float x, float y, float z)
{
	if(x==1.0f&&y==1.0f&&z==1.0f) return;

	float mt[]={
			   x, 0.0f, 0.0f, 0.0f,
			0.0f,    y, 0.0f, 0.0f,
			0.0f, 0.0f,    z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f};

	(*this)=this->operator *(mt);
}

void AEMatrix4f4::Scale(const AEVector3f& vec)
{
	this->Scale(vec.X,vec.Y,vec.Z);
}

void AEMatrix4f4::Transpose(void)
{
	for(int q=0;q<3;q++)
		for(int w=q+1;w<4;w++)
			std::swap(
				this->operator()(q,w),
				this->operator()(w,q)
			);
}

AEMatrix4f4::~AEMatrix4f4(void)
{
}

