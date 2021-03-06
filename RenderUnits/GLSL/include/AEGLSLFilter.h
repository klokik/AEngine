#ifndef AEGLSLFILTER_H_
#define AEGLSLFILTER_H_

#include "AEGLHeader.h"
#include "AEGLSLDerivedPrograms.h"
#include "AEGLRender.h"
#include "AEDebug.h"

#include <memory>

namespace aengine
{
	class _AEGLBuffer
	{
	protected:
		uint32_t _texture_unit;
		int _type;

		void CreateTexture(void)
		{
			AEPrintLog("Create Buffer Texture");
			glGenTextures(1,&_texture_unit);
			glBindTexture(GL_TEXTURE_2D,texture_unit);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			AEGLRenderUnit::CheckError();
		}

	public:

		GLuint const &texture_unit;

		_AEGLBuffer(void):
			texture_unit(_texture_unit)
		{
			_type = GL_RGBA;
			CreateTexture();
		}

		_AEGLBuffer(uint32_t width,uint32_t height,int type):
			texture_unit(_texture_unit)
		{
			_type = type;
			CreateTexture();

			Resize(width,height);
		}

		void SetType(int type)
		{
			_type = type;
		}

		void Resize(uint32_t width,uint32_t height)
		{
			AEPrintLog("Resize GL buffer");
			glBindTexture(GL_TEXTURE_2D,texture_unit);

			int data_type=GL_UNSIGNED_SHORT;
			int format;
			switch(_type)
			{
			case GL_DEPTH_COMPONENT:
				format = GL_DEPTH_COMPONENT;
				data_type=GL_UNSIGNED_SHORT;
				break;
			default:
				format = GL_RGBA;
				data_type=GL_UNSIGNED_BYTE;
			}
			glTexImage2D(GL_TEXTURE_2D,0,_type,width,height,0,format,data_type,NULL);
			AEGLRenderUnit::CheckError();
		}

		operator int ()
		{
			return texture_unit;
		}

		virtual ~_AEGLBuffer(void)
		{
			glDeleteTextures(1,&texture_unit);
		}
	};

	typedef std::shared_ptr<_AEGLBuffer> AEGLBuffer;
}

// class  AEGLSLFilter: public AEGLBuffer
// {
// protected:
// 	AEGLSLProgram2vsquare program;

// public:
// 	bool enabled;


// 	AEGLSLFilter(void):
// 		AEGLBuffer(new _AEGLBuffer)
// 	{
// 		enabled = false;
// 	}

// 	AEGLSLFilter(AEGLBuffer source)
// 	{
// 		*this = source;
// 		enabled = false;
// 	}

// 	AEGLSLFilter(uint32_t width,uint32_t height):
// 		AEGLBuffer(new _AEGLBuffer(width,height))
// 	{
// 		enabled = false;
// 	}


// 	void LoadFromFile(std::string filename)
// 	{
// 		//load shader file into program	
// 		throw 0;
// 	}

// 	AEGLBuffer Apply()
// 	{
// 		if(enabled)
// 		{
// 			//do some stuff
// 		}
		
// 		return *this;
// 	}

// 	virtual ~AEGLSLFilter(void)
// 	{

// 	}
// };

#endif /* AEGLSLFILTER_H_ */