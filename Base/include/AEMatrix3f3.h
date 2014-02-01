/*
 * AEMatrix3f3.h
 *
 *  Created on: Feb 02, 2014
 *      Author: klokik
 */

#ifndef AEMATRIX3F3_H_
#define AEMATRIX3F3_H_

#include <stddef.h>

#include "AEUnits.h"


namespace aengine
{
	class AEMatrix3f3
	{
	protected:
		float data[9];

	public:
		AEMatrix3f3(void);
		AEMatrix3f3(AEMatrix3f3 const &val);
		AEMatrix3f3(float const val[9]);

		void SetIdentity(void);

		float &operator()(size_t col,size_t row);
		float &operator[](size_t id);

		float operator()(size_t col,size_t row) const;
		float operator[](size_t id) const;

		AEMatrix3f3 &operator=(AEMatrix3f3 const &val);
		AEMatrix3f3 &operator=(float const val[9]);

		bool operator==(AEMatrix3f3 val);

		friend AEMatrix3f3 operator*(AEMatrix3f3 const &mA,float val);
		friend AEMatrix3f3 operator*(float val,AEMatrix3f3 const &mA);
		friend AEMatrix3f3 operator*(AEMatrix3f3 const &mA,AEMatrix3f3 const &mB);
		friend AEVector3f operator*(AEMatrix3f3 const &mA,AEVector3f vec);
		friend AEVector3f operator*(AEVector3f vec,AEMatrix3f3 const &mA);

		friend AEMatrix3f3 operator+(AEMatrix3f3 const &mA,AEMatrix3f3 const &mB);
		friend AEMatrix3f3 operator-(AEMatrix3f3 const &mA,AEMatrix3f3 const &mB);

		AEMatrix3f3 operator-(void);

		float const *ToArray(void) const;
		operator const float*() const;

		// AEMatrix3f3 RotateX(float value) const;
		// AEMatrix3f3 RotateY(float value) const;
		// AEMatrix3f3 RotateZ(float value) const;

		// AEMatrix3f3 Translate(AEVector3f vec) const;
		// AEMatrix3f3 Scale(AEVector3f vec) const;

		AEMatrix3f3 Transpose(void) const;

		float Determinant(void) const;
		float Adjunct(size_t col,size_t row) const;

		AEMatrix3f3 Invert(void) const;
	};
}

#endif /* AEMATRIX3F3_H_ */
