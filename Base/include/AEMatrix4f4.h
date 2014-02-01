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


namespace aengine
{
	class AEMatrix4f4
	{
	protected:
		float data[16];

	public:
		AEMatrix4f4(void);
		AEMatrix4f4(AEMatrix4f4 const &val);
		AEMatrix4f4(float const val[16]);

		void SetIdentity(void);

		float &operator()(size_t col,size_t row);
		float &operator[](size_t id);

		float operator()(size_t col,size_t row) const;
		float operator[](size_t id) const;

		AEMatrix4f4 &operator=(AEMatrix4f4 const &val);
		AEMatrix4f4 &operator=(float const val[16]);

		bool operator==(AEMatrix4f4 val);

		friend AEMatrix4f4 operator*(AEMatrix4f4 const &mA,float val);
		friend AEMatrix4f4 operator*(float val,AEMatrix4f4 const &mA);
		friend AEMatrix4f4 operator*(AEMatrix4f4 const &mA,AEMatrix4f4 const &mB);
		friend AEVector4f operator*(AEMatrix4f4 const &mA,AEVector4f vec);
		friend AEVector4f operator*(AEVector4f vec,AEMatrix4f4 const &mA);

		friend AEMatrix4f4 operator+(AEMatrix4f4 const &mA,AEMatrix4f4 const &mB);
		friend AEMatrix4f4 operator-(AEMatrix4f4 const &mA,AEMatrix4f4 const &mB);

		AEMatrix4f4 operator-(void);

		float const *ToArray(void) const;
		operator const float*() const;

		AEMatrix4f4 RotateX(float value) const;
		AEMatrix4f4 RotateY(float value) const;
		AEMatrix4f4 RotateZ(float value) const;

		AEMatrix4f4 Translate(AEVector3f vec) const;
		AEMatrix4f4 Scale(AEVector3f vec) const;

		AEMatrix4f4 Transpose(void) const;

		float Determinant(void) const;
		float Adjunct(size_t col,size_t row) const;

		AEMatrix4f4 Invert(void) const;
	};
}

#endif /* AEMATRIX4F4_H_ */
