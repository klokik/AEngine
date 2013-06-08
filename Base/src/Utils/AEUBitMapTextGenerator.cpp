/*
 * AEUBitMapTextGenerator.cpp
 *
 *  Created on: Dec 24, 2011
 *      Author: klokik
 */

#include <string.h>
#include <vector>

#include "Utils/AEUBitMapTextGenerator.h"
#include "AESysutils.h"

#define COLOR2INT(i) ((uint8_t)i.R)<<0|((uint8_t)i.G)<<8|((uint8_t)i.B)<<16|((uint8_t)i.A<<24)

AEUBitMapTextGenerator::AEUBitMapTextGenerator()
{
	layout=NULL;
	this->width=this->height=0;
	this->map=(AETexture*)malloc(sizeof(AETexture));
	memset(this->map,0,sizeof(AETexture));
}

AETexture *AEUBitMapTextGenerator::GenTexture(AETextBlock &text)
{
	AETexture *tex=NULL;

	if(text.items.size()&&this->width&&this->height)
	{
		tex=(AETexture*)malloc(sizeof(AETexture));

		tex->bpp=32;
		tex->pixelformat=AE_PF_RGBA;

		tex->width=this->map->width/this->width*text.items[0].text.length();
		tex->height=this->map->height/this->height;
		tex->size=tex->bpp/8*tex->width*tex->height;
		tex->data=malloc(tex->size);

		memset32(tex->data,COLOR2INT(text.items[0].bgcolor),tex->size);

		this->PutItem(tex,text.items[0]);
	}

	return tex;
}

AEMesh *AEUBitMapTextGenerator::GenMesh(AETextBlock &text)
{
	AEMesh *mesh=NULL;
	if(text.items.size())
	{
		float offsetx=0;
		float offsety=0;
		float cW=1.0f/this->width;
		float cH=1.0f/this->height;
		int ci=0;	//current item
		mesh=new AEMesh;

		std::vector<AEVertexf>  vtx;
		std::vector<AETexCoord> tcr;
		std::vector<AETriangle> fce;

		for(uint q=0;q<text.items[ci].text.length();q++)
		{
			int cx=text.items[ci].text[q]%this->width;
			int cy=text.items[ci].text[q]/this->width;

			vtx.push_back({offsetx,                     offsety,                     0.0f});
			vtx.push_back({offsetx+text.items[ci].size, offsety,                     0.0f});
			vtx.push_back({offsetx+text.items[ci].size, offsety+text.items[ci].size, 0.0f});
			vtx.push_back({offsetx,                     offsety+text.items[ci].size, 0.0f});

			offsetx+=text.items[ci].size+text.items[ci].spacing;

			fce.push_back({q*4,q*4+1,q*4+2});
			fce.push_back({q*4,q*4+3,q*4+2});

			tcr.push_back({cx*cW,    (cy+1)*cH,0.0f});
			tcr.push_back({(cx+1)*cW,(cy+1)*cH,0.0f});
			tcr.push_back({(cx+1)*cW,(cy)*cH,0.0f});
			tcr.push_back({cx*cW,    (cy)*cH,0.0f});
		}

		mesh->vtxcount=vtx.size();
		mesh->vtx=(AEVector3f*)malloc(sizeof(AEVector3f)*mesh->vtxcount);
		memcpy(mesh->vtx,&vtx[0],sizeof(AEVector3f)*mesh->vtxcount);

		mesh->fcecount=fce.size();
		mesh->fce=(AETriangle*)malloc(sizeof(AETriangle)*mesh->fcecount);
		memcpy(mesh->fce,&fce[0],sizeof(AETriangle)*mesh->fcecount);

		mesh->tcrcount=tcr.size();
		mesh->tcr=(AETexCoord*)malloc(sizeof(AETexCoord)*mesh->tcrcount);
		memcpy(mesh->tcr,&tcr[0],sizeof(AETexCoord)*mesh->tcrcount);

		mesh->nrmcount=vtx.size();
		mesh->nrm=(AEVector3f*)malloc(sizeof(AEVector3f)*mesh->vtxcount);
		for(size_t q=0;q<mesh->nrmcount;mesh->nrm[q]={0.0f,0.0f,1.0f},q++);
	}

	return mesh;
}

void AEUBitMapTextGenerator::PutItem(AETexture *tex,_aet_item item)
{
	//TODO:Optimize this shit
	if(this->map)
	{
		uint16_t SymW=map->width/this->width;
		uint16_t SymH=map->height/this->height;


		for(size_t l=0;l<item.text.length();l++)
		{
			char ch=item.text[l];

			for(uint16_t q=0;q<SymH;q++)
				for(uint16_t w=0;w<SymW;w++)
				{
					uint32_t id=(q+(ch/this->width)*SymH)*map->width+w+ch%this->width*SymW;
					uint32_t pixel=0;
					switch(map->bpp)
					{
					case 8:
						{
							uint8_t tmp=((uint8_t*)map->data)[id];
							pixel=0xFF000000|tmp<<16|tmp<<8|tmp<<0;
						}
						break;
					case 24:
						{
							pixel=((uint8_t*)map->data)[id]|0xFF000000;
						}
						break;
					case 32:
						pixel=((uint32_t*)map->data)[id];
						break;
					}
					((uint32_t*)tex->data)[q*tex->width+w+l*SymW]=pixel;
				}
		}
	}
}

AEUBitMapTextGenerator::~AEUBitMapTextGenerator()
{
	if(this->map!=NULL)
		free(this->map);
}

