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
#include <stdexcept>

#define fdcos(a) (float)cos(a/57.2957795f)
#define fdsin(a) (float)sin(a/57.2957795f)


namespace aengine
{
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

	float &AEMatrix4f4::operator ()(size_t col,size_t row)
	{
		return this->data[col*4+row];
	}

	float &AEMatrix4f4::operator [](size_t id)
	{
		return this->data[id];
	}

	float AEMatrix4f4::operator()(size_t col,size_t row) const
	{
		return this->data[col*4+row];
	}

	float AEMatrix4f4::operator[](size_t id) const
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

	bool AEMatrix4f4::operator ==(AEMatrix4f4 val)
	{
		return memcmp((const void*)this->data,(const void*)val.data,sizeof(float)*16);
	}

	AEMatrix4f4 operator *(AEMatrix4f4 const &mA,float val)
	{
		float ret_val[16];

		for(size_t q=0;q<16;q++)
			ret_val[q]=mA.data[q]*val;

		return AEMatrix4f4(ret_val);
	}

	AEMatrix4f4 operator *(float val,AEMatrix4f4 const &mA)
	{
		return operator *(mA,val);
	}

	AEMatrix4f4 operator *(AEMatrix4f4 const &mA,AEMatrix4f4 const &mB)
	{
		/*
		 * +----------------+
		 * | 00  04  08  12 |
		 * | 01  05  09  13 |
		 * | 02  06  10  14 |
		 * | 03  07  11  15 |
		 * +----------------+
		 */
		
		const float *a=mA.data;
		const float *b=mB.data;

		float ret_val[16];

		for(int q=0;q<16;q+=4)
			for(int w=0;w<4;w++)
				ret_val[q+w]=a[w]*b[q]+a[w+4]*b[q+1]+a[w+8]*b[q+2]+a[w+12]*b[q+3];


		return AEMatrix4f4(ret_val);
	}

	AEVector4f operator *(AEMatrix4f4 const &mA,AEVector4f vec)
	{
		AEVector4f res;
		const float *a=mA.data;

		res.X=a[0]*vec.X+a[4]*vec.Y+a[ 8]*vec.Z+a[12]*vec.W;
		res.Y=a[1]*vec.X+a[5]*vec.Y+a[ 9]*vec.Z+a[13]*vec.W;
		res.Z=a[2]*vec.X+a[6]*vec.Y+a[10]*vec.Z+a[14]*vec.W;
		res.W=a[3]*vec.X+a[7]*vec.Y+a[11]*vec.Z+a[15]*vec.W;

		return res;
	}

	AEMatrix4f4 operator+(AEMatrix4f4 const &mA,AEMatrix4f4 const &mB)
	{
		const float *a=mA.data;
		const float *b=mB.data;
		float ret_val[16];

		for(size_t q=0;q<16;q++)
			ret_val[q]=a[q]+b[q];


		return AEMatrix4f4(ret_val);
	}

	AEMatrix4f4 operator-(AEMatrix4f4 const &mA,AEMatrix4f4 const &mB)
	{
		const float *a=mA.data;
		const float *b=mB.data;
		float ret_val[16];

		for(size_t q=0;q<16;q++)
			ret_val[q]=a[q]-b[q];


		return AEMatrix4f4(ret_val);
	}

	AEMatrix4f4 AEMatrix4f4::operator-(void)
	{
		return (*this)*(-1.0f);
	}

	float const *AEMatrix4f4::ToArray(void) const
	{
		return this->data;
	}

	AEMatrix4f4::operator const float*() const
	{
		return this->ToArray();
	}

	AEMatrix4f4 AEMatrix4f4::RotateX(float value) const
	{
		if(value==0.0f)
			return *this;

		float mt[]={
				1.0f, 0.0f,         0.0f,         0.0f,
				0.0f, fdcos(value), fdsin(value), 0.0f,
				0.0f,-fdsin(value), fdcos(value), 0.0f,
				0.0f, 0.0f,         0.0f,         1.0f};

		return (*this)*AEMatrix4f4(mt);
	}

	AEMatrix4f4 AEMatrix4f4::RotateY(float value) const
	{
		if(value==0.0f)
			return *this;

		float mt[]={
				fdcos(value), 0.0f,-fdsin(value), 0.0f,
				0.0f,         1.0f, 0.0f,         0.0f,
				fdsin(value), 0.0f, fdcos(value), 0.0f,
				0.0f,         0.0f, 0.0f,         1.0f};

		return (*this)*AEMatrix4f4(mt);
	}

	AEMatrix4f4 AEMatrix4f4::RotateZ(float value) const
	{
		if(value==0.0f)
			return *this;

		float mt[]={
				fdcos(value), fdsin(value), 0.0f, 0.0f,
				-fdsin(value),fdcos(value), 0.0f, 0.0f,
				0.0f,         0.0f,         1.0f, 0.0f,
				0.0f,         0.0f,         0.0f, 1.0f};

		return (*this)*AEMatrix4f4(mt);
	}

	AEMatrix4f4 AEMatrix4f4::Translate(AEVector3f v) const
	{
		float mt[]={
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				 v.X,  v.Y,  v.Z, 1.0f};

		return (*this)*AEMatrix4f4(mt);
	}

	AEMatrix4f4 AEMatrix4f4::Scale(AEVector3f v) const
	{
		if(v.X==1.0f&&v.Y==1.0f&&v.Z==1.0f)
			return *this;

		float mt[]={
				 v.X, 0.0f, 0.0f, 0.0f,
				0.0f,  v.Y, 0.0f, 0.0f,
				0.0f, 0.0f,  v.Z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f};

		return (*this)*AEMatrix4f4(mt);
	}

	AEMatrix4f4 AEMatrix4f4::Transpose(void) const
	{
		float ret_val[16];

		for(int q=0;q<4;q++)
			for(int w=0;w<4;w++)
				ret_val[q*4+w] = data[w*4+q];


		return AEMatrix4f4(ret_val);
	}

	float AEMatrix4f4::Determinant(void) const
	{
		float sum=0.0f;

		for(size_t q=0;q<4;q++)
			sum+=(*this)(q,0)*Adjunct(q,0);


		return sum;
	}

	float AEMatrix4f4::Adjunct(size_t col,size_t row) const
	{
		float fdata[16];

		for(size_t q=0;q<4;q++)
			for(size_t w=0;w<4;w++)
			{
				size_t _col=q-(q>col);
				size_t _row=w-(w>row);

				fdata[_col*4+_row]=(*this)(q,w);
			}

		float sum=
			fdata[0]*fdata[5]*fdata[10]+
			fdata[4]*fdata[9]*fdata[2]+
			fdata[8]*fdata[1]*fdata[6]-
			fdata[8]*fdata[5]*fdata[2]-
			fdata[4]*fdata[1]*fdata[10]-
			fdata[0]*fdata[9]*fdata[6];

		return sum*(1-static_cast<int>((col+row)%2)*2);
	}

	AEMatrix4f4 AEMatrix4f4::Invert(void) const
	{
		float det=Determinant();
		if(det==0)
			throw std::invalid_argument("given matrix is not inversible");

		float ret_val[16];

		for(int q=0;q<16;q++)
			ret_val[q]=Adjunct(q/4,q%4);


		return AEMatrix4f4(ret_val).Transpose()*(1/det);
	}
}
