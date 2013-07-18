/*
 * AEUnits.h
 *
 *  Created on: Aug 21, 2011
 *      Author: klokik
 */

#ifndef AEUNITS_H_
#define AEUNITS_H_

#include "AEDefines.h"


typedef struct
{
	float X;
	float Y;
}AEVector2f;

typedef struct
{
	float X;
	float Y;
	float Z;
}AEVector3f;

typedef struct
{
	float X;
	float Y;
	float Z;
	float W;
}AEVector4f;

typedef AEVector3f AEVertexf;

typedef AEVector2f Vec2f;
typedef AEVector3f Vec3f;
typedef AEVector4f Vec4f;

typedef struct
{
	union
	{
		struct
		{
			float U;
			float V;
			float W;
		};
		AEVector3f vec;
	};
}AETexCoord;

typedef struct
{
	unsigned int A;
	unsigned int B;
	unsigned int C;
}AETriangle;

typedef struct
{
	union
	{
		struct
		{
			float R;
			float G;
			float B;
			float A;
		};
		AEVector4f vec;
	};
}AEColor;

#endif /* AEUNITS_H_ */
