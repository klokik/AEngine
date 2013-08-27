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
		std::string file_name=work_directory+res_name;

		img.width=256;
		img.height=256;
		img.bpp=4*8;

		img.size=img.width*img.height*img.bpp/8;

		img.data=malloc(img.size);

		uint8_t *data=static_cast<uint8_t*>(img.data);
		// fill with some random data
		int q=0;
		for(size_t q=0;q<img.size;q++)//uint8_t *pt=data;pt!=data+img.size;pt++,q++)
		{
			data[q]=q%256;
			//*pt=q%256;
		}

		img.cached=false;

		return AE_OK;
	}
}