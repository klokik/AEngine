#include <fstream>

#include <png.h>

#include "AEResourceManager.h"
#include "AEDebug.h"


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

		const size_t header_size=8;	// 8 is the maximum size that can be checked
		char header[header_size];

		// open and validate file
		std::ifstream if_str;
		if_str.open(file_name,std::ifstream::in);

		if(!if_str.good())
		{
			AEPrintLog("[read_png_file] File \""+file_name+"\" could not be opened for reading");
			return AE_ERR;
		}

		if_str.read(header,header_size);

		if(png_sig_cmp((png_const_bytep)header,0,header_size))
		{
			AEPrintLog("[read_png_file] File \""+file_name+"\" is not recognized as a PNG file");
			return AE_ERR;
		}


		/* initialize stuff */
		png_structp png_ptr=png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);

		if(!png_ptr)
		{
			AEPrintLog("[read_png_file] png_create_read_struct failed");
			return AE_ERR;
		}

		png_infop info_ptr=png_create_info_struct(png_ptr);
		if(!info_ptr)
		{
			png_destroy_read_struct(&png_ptr,&info_ptr,(png_infopp)0);
			AEPrintLog("[read_png_file] png_create_info_struct failed");
			return AE_ERR;
		}

		if(setjmp(png_jmpbuf(png_ptr)))
		{
			png_destroy_read_struct(&png_ptr,&info_ptr,(png_infopp)0);
			AEPrintLog("[read_png_file] Error during init_io");
			return AE_ERR;
		}


		// png_init_io(png_ptr,fp);
		png_set_read_fn(png_ptr,&if_str,
			[](png_structp png_ptr,png_bytep data,png_size_t length)->void
			{
				std::istream *if_str=static_cast<std::istream*>(png_get_io_ptr(png_ptr));
				if_str->read((char*)data,length);
			});
		png_set_sig_bytes(png_ptr,header_size);

		png_read_info(png_ptr,info_ptr);

		unsigned int width=png_get_image_width(png_ptr,info_ptr);
		unsigned int height=png_get_image_height(png_ptr,info_ptr);
		// png_byte color_type=png_get_color_type(png_ptr,info_ptr);
		png_byte bit_depth=png_get_bit_depth(png_ptr,info_ptr);
		png_byte channels=png_get_channels(png_ptr,info_ptr);

		// int number_of_passes=png_set_interlace_handling(png_ptr);
		png_read_update_info(png_ptr,info_ptr);


		img.width=width;
		img.height=height;
		img.bpp=bit_depth*channels;
		img.size=img.width*img.height*img.bpp/8;
		img.cached=false;

		img.data=malloc(img.size);

		/* read file */
		if(setjmp(png_jmpbuf(png_ptr)))
		{
			png_destroy_read_struct(&png_ptr,&info_ptr,(png_infopp)0);
			AEPrintLog("[read_png_file] Error during read_image");
			return AE_ERR;
		}

		size_t row_size=png_get_rowbytes(png_ptr,info_ptr);

		png_bytep *row_pointers=new png_bytep[height];
		for(size_t q=0;q<height;q++)
			row_pointers[q]=static_cast<png_byte*>(img.data)+row_size*q;

		png_read_image(png_ptr,row_pointers);

		delete []row_pointers;
		png_destroy_read_struct(&png_ptr,&info_ptr,(png_infopp)0);

		if_str.close();

		return AE_OK;
	}
}