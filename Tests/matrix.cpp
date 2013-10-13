#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

#include "AEMatrix4f4.h"
#include "AEVectorMath.h"

using namespace aengine;

std::string ToString(AEMatrix4f4 const &m)
{
	std::stringstream data;

	data
	<<"|"<<m[0]<<"\t,"<<m[4]<<"\t,"<<m[8]<<"\t,"<<m[12]<<"\t|\n"
	<<"|"<<m[1]<<"\t,"<<m[5]<<"\t,"<<m[9]<<"\t,"<<m[13]<<"\t|\n"
	<<"|"<<m[2]<<"\t,"<<m[6]<<"\t,"<<m[10]<<"\t,"<<m[14]<<"\t|\n"
	<<"|"<<m[3]<<"\t,"<<m[7]<<"\t,"<<m[11]<<"\t,"<<m[15]<<"\t|";

	return data.str();
}

int main(void)
{
	AEMatrix4f4 A;
	AEMatrix4f4 B;
	AEMatrix4f4 R;

	const AEMatrix4f4 &C=A;
	const AEMatrix4f4 &D=B;

	try
	{
		R = A.RotateX(60).RotateY(16).RotateZ(67);
		B = B.Translate(vec3f(20,1,20));

		R = A*B;
		R = C*D;
		R = A*A;
		R = A*C;
		R = A*D;
		R = C*B;
		R = A*B*C + D*2.0f*A - 4*C*A;
		R = -R;

		float mtx[] = {1,3,5,7,11,13,17,19,23,31,37,41,43,47,51,57};
		A = mtx;
		R = A.Invert();

		if(A.Determinant()!=888)
			throw 1;

		if(fabs((A*R).Determinant()-1.0f)>0.001f)
			throw 2;

		try
		{
			float mtx[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
			A = mtx;

			// should fail
			A.Invert();
			throw 3;
		}
		catch(const std::invalid_argument &except)
		{
			std::cout<<"OK"<<std::endl;
		}
	}
	catch(int code)
	{
		std::cout<<"Failed test "<<code<<std::endl;
		//return 1;
	}

	std::cout<<ToString(A)<<std::endl;
	std::cout<<"det(R*R^-1): "<<(R*R.Invert()).Determinant()<<std::endl;

	std::cout<<"OK"<<std::endl;
	return 0;
}