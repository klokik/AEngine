#include <stdlib.h>

#include "AEResourceManager.h"


namespace aengine
{
	std::string AEResourceManager::work_directory;

	int AEResourceManager::LoadImage(std::string res_name,AEImage &img,std::string type)
	{
		return LoadImagePng(res_name,img);
	}

	int AEResourceManager::LoadImagePng(std::string res_name,AEImage &img)
	{
		std::string filename=work_directory+res_name;

		img.width=256;
		img.height=256;
		img.bpp=4*8;

		img.size=img.width*img.height*img.bpp/8;

		img.data=malloc(img.size);

		uint8_t *data=static_cast<uint8_t*>(img.data);
		// fill with some random data
		int q=0;
		for(uint8_t *pt=data;pt!=data+img.size;pt++,q++)
		{
			*pt=128;//q%256;
		}

		return AE_OK;
	}
}