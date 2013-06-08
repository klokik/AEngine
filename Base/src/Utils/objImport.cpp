/*
 * objImport.cpp
 *
 *  Created on: Oct 8, 2011
 *      Author: klokik
 */

#ifndef OBJIMPORT_H_
#define OBJIMPORT_H_

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <vector>

#include "AEUnits.h"
#include "AEMesh.h"

void LoadObjFile(AEMesh *&mesh, const char *path)
{
	FILE *f;
	f=fopen(path,"r");

	if(f!=NULL)
	{
		const size_t buf_size=1024;
		char str[buf_size];

		std::vector<AEVertexf> vvtx;
		std::vector<AETriangle> vfce;
		std::vector<AETexCoord> vtcr;
		std::vector<AEVector3f> vnrm;

		mesh=new AEMesh;

		bool ffirst=true;
		bool unorm=false;
		bool utcr= false;

		while(fgets(str,buf_size,f))
		{
			if(strnlen(str,buf_size)>1)
			{
				switch(str[0])
				{
				case 'v':
				{
					switch(str[1])
					{
					case ' ':
					{
						AEVertexf vtx;
						strtok(str," ");

						vtx.X=atof(strtok(NULL," "));
						vtx.Y=atof(strtok(NULL," "));
						vtx.Z=atof(strtok(NULL," "));

						vvtx.push_back(vtx);
						break;
					}
					case 't':
					{
						AETexCoord tc;
						strtok(str," ");

						tc.U=atof(strtok(NULL," "));
						tc.V=atof(strtok(NULL," "));

						vtcr.push_back(tc);
						utcr=true;
						break;
					}
					case 'n':
					{
						AEVertexf nrm;
						strtok(str," ");

						nrm.X=atof(strtok(NULL," "));
						nrm.Y=atof(strtok(NULL," "));
						nrm.Z=atof(strtok(NULL," "));

						vnrm.push_back(nrm);
						unorm=true;
						break;
					}
					}
					break;
				}
				case 'f':
				{
					if(ffirst)
					{
						ffirst=false;
						if(vvtx.size()) mesh->vtx=(AEVertexf*)malloc(sizeof(AEVertexf)*vvtx.size());
						if(vtcr.size()) mesh->tcr=(AETexCoord*)malloc(sizeof(AETexCoord)*vvtx.size());
						if(vnrm.size()) mesh->nrm=(AEVector3f*)malloc(sizeof(AEVector3f)*vvtx.size());

						mesh->vtxcount=vvtx.size();
						mesh->tcrcount=vvtx.size();
						mesh->nrmcount=vvtx.size();

						memcpy(mesh->vtx,&vvtx[0],sizeof(AEVertexf)*vvtx.size());
					}

					AETriangle fce;
					memset(&fce,0,sizeof(AETriangle));

					strtok(str," ");
					char *sustr[3];
					sustr[0]=strtok(NULL," ");
					sustr[1]=strtok(NULL," ");
					sustr[2]=strtok(NULL," ");

					int A=fce.A=atoi(sustr[0])-1;
					int B=fce.B=atoi(sustr[1])-1;
					int C=fce.C=atoi(sustr[2])-1;

					if(strchr(sustr[0],'/'))
					{
						int tcrid[3];
						int nrmid[3];

						strtok(sustr[0],"/");
						tcrid[0]=atoi(strtok(NULL,"/"))-1;
						nrmid[0]=atoi(strtok(NULL,"/"))-1;

						strtok(sustr[1],"/");
						tcrid[1]=atoi(strtok(NULL,"/"))-1;
						nrmid[1]=atoi(strtok(NULL,"/"))-1;

						strtok(sustr[2],"/");
						tcrid[2]=atoi(strtok(NULL,"/"))-1;
						nrmid[2]=atoi(strtok(NULL,"/"))-1;

						if(utcr)
						{
							mesh->tcr[A]=vtcr[tcrid[0]];
							mesh->tcr[B]=vtcr[tcrid[1]];
							mesh->tcr[C]=vtcr[tcrid[2]];
						}

						if(unorm)
						{
							mesh->nrm[A]=vnrm[nrmid[0]];
							mesh->nrm[B]=vnrm[nrmid[1]];
							mesh->nrm[C]=vnrm[nrmid[2]];
						}
					}
//						int id,idn;
//
//						if(!unorm)
//						{
//							fce.A=atoi(strtok(NULL," /"))-1;
//							id=atoi(strtok(NULL," /"))-1;
//							if(utcr) mesh->tcr[fce.A]=vtcr[id];
//
//							fce.B=atoi(strtok(NULL," /"))-1;
//							id=atoi(strtok(NULL," /"))-1;
//							if(utcr) mesh->tcr[fce.B]=vtcr[id];
//
//							fce.C=atoi(strtok(NULL," /"))-1;
//							id=atoi(strtok(NULL," /"))-1;
//							if(utcr) mesh->tcr[fce.C]=vtcr[id];
//						}
//						else
//						{
//							fce.A=atoi(strtok(NULL," /"))-1;
//							id=atoi(strtok(NULL," /"))-1;
//							if(utcr) mesh->tcr[fce.A]=vtcr[id];
//							idn=atoi(strtok(NULL," /"))-1;
//							mesh->nrm[id]=vnrm[idn];
//
//							fce.B=atoi(strtok(NULL," /"))-1;
//							id=atoi(strtok(NULL," /"))-1;
//							if(utcr) mesh->tcr[fce.B]=vtcr[id];
//							idn=atoi(strtok(NULL," /"))-1;
//							mesh->nrm[id]=vnrm[idn];
//
//							fce.C=atoi(strtok(NULL," /"))-1;
//							id=atoi(strtok(NULL," /"))-1;
//							if(utcr) mesh->tcr[fce.C]=vtcr[id];
//							idn=atoi(strtok(NULL," /"))-1;
//							mesh->nrm[id]=vnrm[idn];
//						}
//					}
//					else
//					{
//						strtok(str," ");
//
//						fce.A=atoi(strtok(NULL," "))-1;
//						fce.B=atoi(strtok(NULL," "))-1;
//						fce.C=atoi(strtok(NULL," "))-1;
//					}

					vfce.push_back(fce);
					break;
				}
				}
			}
		}

		size_t size=sizeof(AETriangle)*vfce.size();
		void *mem=malloc(size);
		mesh->fce=(AETriangle*)mem;
		memcpy(mesh->fce,&vfce[0],size);
		mesh->fcecount=vfce.size();

		printf("Vertex: %d;\nFace: %d\nNormals: %d\nTexCoords: %d\n",mesh->vtxcount,mesh->fcecount,mesh->nrmcount,mesh->tcrcount);
		fclose(f);
	}
}

#endif
