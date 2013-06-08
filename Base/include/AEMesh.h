/*
 * AEMesh.h
 *
 *  Created on: Feb 25, 2012
 *      Author: klokik
 */

#ifndef AEMESH_H_
#define AEMESH_H_

#include "AEUnits.h"

#define AE_UPDATE_VERTEX	0x0001
#define AE_UPDATE_TEXCOORD	0x0002
#define AE_UPDATE_NORMAL	0x0004
#define AE_UPDATE_INDEX		0x0008

 #define AE_UPDATE_ALL		(AE_UPDATE_VERTEX|AE_UPDATE_TEXCOORD|AE_UPDATE_NORMAL|AE_UPDATE_INDEX)

class AEMesh
{
private:
	unsigned int invalid;

public:
	AEVertexf *vtx;		//Vertexes
	AETexCoord *tcr;	//Texture coordinates
	AEVector3f *nrm;	//Normals
	union
	{
		AETriangle *fce;	//Faces
		unsigned int *ind;	//Vertex indexes
	};

	unsigned int vtxcount;
	unsigned int tcrcount;
	unsigned int nrmcount;
	union
	{
		unsigned int fcecount;
		unsigned int indcount;
	};

	unsigned int idvtx;
	unsigned int idtcr;
	unsigned int idnrm;
	union
	{
		unsigned int idfce;
		unsigned int idind;
	};

	bool cached;

	AEMesh(void);

	void Invalidate(unsigned int mask);
	void Validate(unsigned int mask);
	unsigned int IsInvalid(void);

	AEMesh Clone(void);
	void CloneFrom(AEMesh &mesh);

	AEMesh &operator = (AEMesh &mesh);

	void Free(void);

	virtual ~AEMesh(void);
};

#endif /* AEMESH_H_ */
