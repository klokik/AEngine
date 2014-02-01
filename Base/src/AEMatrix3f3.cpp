/*
 * AEMatrix3f3.cpp
 *
 *  Created on: Feb 02, 2014
 *      Author: klokik
 */

#include "AEMatrix3f3.h"

#include <string.h>
#include <math.h>
#include <algorithm>
#include <stdexcept>

// #define fdcos(a) (float)cos(a/57.2957795f)
// #define fdsin(a) (float)sin(a/57.2957795f)


namespace aengine
{
	AEMatrix3f3::AEMatrix3f3(void)
	{
		this->SetIdentity();
	}

	AEMatrix3f3::AEMatrix3f3(AEMatrix3f3 const &val)
	{
		this->operator =(val);
	}

	AEMatrix3f3::AEMatrix3f3(float const val[])
	{
		this->operator =(val);
	}

	void AEMatrix3f3::SetIdentity(void)
	{
		memset((void*)this->data,0,sizeof(float)*9);
		this->data[0]=1.0f;
		this->data[4]=1.0f;
		this->data[8]=1.0f;
	}

	float &AEMatrix3f3::operator ()(size_t col,size_t row)
	{
		return this->data[col*3+row];
	}

	float &AEMatrix3f3::operator [](size_t id)
	{
		return this->data[id];
	}

	float AEMatrix3f3::operator()(size_t col,size_t row) const
	{
		return this->data[col*3+row];
	}

	float AEMatrix3f3::operator[](size_t id) const
	{
		return this->data[id];
	}

	AEMatrix3f3 &AEMatrix3f3::operator =(AEMatrix3f3 const &val)
	{
		memcpy((void*)this->data,(void*)val.data,sizeof(float)*9);
		return *this;
	}

	AEMatrix3f3 &AEMatrix3f3::operator =(float const val[9])
	{
		memcpy((void*)this->data,(void*)val,sizeof(float)*9);
		return *this;
	}

	bool AEMatrix3f3::operator ==(AEMatrix3f3 val)
	{
		return memcmp((const void*)this->data,(const void*)val.data,sizeof(float)*9);
	}

	AEMatrix3f3 operator *(AEMatrix3f3 const &mA,float val)
	{
		float ret_val[9];

		for(size_t q=0;q<9;q++)
			ret_val[q]=mA.data[q]*val;

		return AEMatrix3f3(ret_val);
	}

	AEMatrix3f3 operator *(float val,AEMatrix3f3 const &mA)
	{
		return operator *(mA,val);
	}

	AEMatrix3f3 operator *(AEMatrix3f3 const &mA,AEMatrix3f3 const &mB)
	{
		/*
		 * +------------+
		 * | 00  03  06 |
		 * | 01  04  07 |
		 * | 02  05  08 |
		 * +------------+
		 */
		
		const float *a=mA.data;
		const float *b=mB.data;

		float ret_val[9];

		for(int q=0;q<9;q+=3)
			for(int w=0;w<3;w++)
				ret_val[q+w]=a[w]*b[q]+a[w+3]*b[q+1]+a[w+6]*b[q+2];


		return AEMatrix3f3(ret_val);
	}

	AEVector3f operator *(AEMatrix3f3 const &mA,AEVector3f vec)
	{
		AEVector3f res;
		const float *a=mA.data;

		res.X=a[0]*vec.X+a[3]*vec.Y+a[6]*vec.Z;
		res.Y=a[1]*vec.X+a[4]*vec.Y+a[7]*vec.Z;
		res.Z=a[2]*vec.X+a[5]*vec.Y+a[8]*vec.Z;

		return res;
	}

	AEVector3f operator*(AEVector3f vec,AEMatrix3f3 const &mA)
	{
		AEVector3f res;
		const float *a=mA.data;

		res.X=a[0]*vec.X+a[1]*vec.Y+a[2]*vec.Z;
		res.Y=a[3]*vec.X+a[4]*vec.Y+a[5]*vec.Z;
		res.Z=a[6]*vec.X+a[7]*vec.Y+a[8]*vec.Z;

		// actually the result is a transposed vector ... but who cares
		return res;
	}

	AEMatrix3f3 operator+(AEMatrix3f3 const &mA,AEMatrix3f3 const &mB)
	{
		const float *a=mA.data;
		const float *b=mB.data;
		float ret_val[9];

		for(size_t q=0;q<9;q++)
			ret_val[q]=a[q]+b[q];


		return AEMatrix3f3(ret_val);
	}

	AEMatrix3f3 operator-(AEMatrix3f3 const &mA,AEMatrix3f3 const &mB)
	{
		const float *a=mA.data;
		const float *b=mB.data;
		float ret_val[9];

		for(size_t q=0;q<9;q++)
			ret_val[q]=a[q]-b[q];


		return AEMatrix3f3(ret_val);
	}

	AEMatrix3f3 AEMatrix3f3::operator-(void)
	{
		return (*this)*(-1.0f);
	}

	float const *AEMatrix3f3::ToArray(void) const
	{
		return this->data;
	}

	AEMatrix3f3::operator const float*() const
	{
		return this->ToArray();
	}

	// AEMatrix3f3 AEMatrix3f3::RotateX(float value) const
	// {
	// 	if(value==0.0f)
	// 		return *this;

	// 	float mt[]={
	// 			1.0f, 0.0f,         0.0f,         0.0f,
	// 			0.0f, fdcos(value), fdsin(value), 0.0f,
	// 			0.0f,-fdsin(value), fdcos(value), 0.0f,
	// 			0.0f, 0.0f,         0.0f,         1.0f};

	// 	return (*this)*AEMatrix3f3(mt);
	// }

	// AEMatrix3f3 AEMatrix3f3::RotateY(float value) const
	// {
	// 	if(value==0.0f)
	// 		return *this;

	// 	float mt[]={
	// 			fdcos(value), 0.0f,-fdsin(value), 0.0f,
	// 			0.0f,         1.0f, 0.0f,         0.0f,
	// 			fdsin(value), 0.0f, fdcos(value), 0.0f,
	// 			0.0f,         0.0f, 0.0f,         1.0f};

	// 	return (*this)*AEMatrix3f3(mt);
	// }

	// AEMatrix3f3 AEMatrix3f3::RotateZ(float value) const
	// {
	// 	if(value==0.0f)
	// 		return *this;

	// 	float mt[]={
	// 			fdcos(value), fdsin(value), 0.0f, 0.0f,
	// 			-fdsin(value),fdcos(value), 0.0f, 0.0f,
	// 			0.0f,         0.0f,         1.0f, 0.0f,
	// 			0.0f,         0.0f,         0.0f, 1.0f};

	// 	return (*this)*AEMatrix3f3(mt);
	// }

	// AEMatrix3f3 AEMatrix3f3::Translate(AEVector3f v) const
	// {
	// 	float mt[]={
	// 			1.0f, 0.0f, 0.0f, 0.0f,
	// 			0.0f, 1.0f, 0.0f, 0.0f,
	// 			0.0f, 0.0f, 1.0f, 0.0f,
	// 			 v.X,  v.Y,  v.Z, 1.0f};

	// 	return (*this)*AEMatrix3f3(mt);
	// }

	// AEMatrix3f3 AEMatrix3f3::Scale(AEVector3f v) const
	// {
	// 	if(v.X==1.0f&&v.Y==1.0f&&v.Z==1.0f)
	// 		return *this;

	// 	float mt[]={
	// 			 v.X, 0.0f, 0.0f, 0.0f,
	// 			0.0f,  v.Y, 0.0f, 0.0f,
	// 			0.0f, 0.0f,  v.Z, 0.0f,
	// 			0.0f, 0.0f, 0.0f, 1.0f};

	// 	return (*this)*AEMatrix3f3(mt);
	// }

	AEMatrix3f3 AEMatrix3f3::Transpose(void) const
	{
		float ret_val[9];

		for(int q=0;q<3;q++)
			for(int w=0;w<3;w++)
				ret_val[q*3+w] = data[w*3+q];


		return AEMatrix3f3(ret_val);
	}

	float AEMatrix3f3::Determinant(void) const
	{
		float sum=0.0f;

		for(size_t q=0;q<3;q++)
			sum+=(*this)(q,0)*Adjunct(q,0);


		return sum;
	}

	float AEMatrix3f3::Adjunct(size_t col,size_t row) const
	{
		float fdata[9];

		for(size_t q=0;q<3;q++)
			for(size_t w=0;w<3;w++)
			{
				size_t _col=q-(q>col);
				size_t _row=w-(w>row);

				fdata[_col*3+_row]=(*this)(q,w);
			}

		float sum=fdata[0]*fdata[4]-fdata[1]*fdata[3];

		return sum*(1-static_cast<int>((col+row)%2)*2);
	}

	AEMatrix3f3 AEMatrix3f3::Invert(void) const
	{
		float det=Determinant();
		if(det==0)
			throw std::invalid_argument("given matrix is not inversible");

		float ret_val[9];

		for(int q=0;q<9;q++)
			ret_val[q]=Adjunct(q/3,q%3);


		return AEMatrix3f3(ret_val).Transpose()*(1/det);
	}
}
