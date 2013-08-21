/*
 * AEGLSLRender.cpp
 *
 *  Created on: Jan 31, 2012
 *      Author: klokik
 */

#include <assert.h>
#include <stdlib.h>

#include "AEGLHeader.h"
#include "AEGLSLRender.h"
#include "AEIO.h"
#include "AEGLSLFilePath.h"
#include "AEVectorMath.h"
#include "AEDebug.h"

// Temporary
#include "../resource/shader/builtin.h"


namespace aengine
{
	AEGLSLRenderUnit::AEGLSLRenderUnit()
	{
		use_MRT=true;
		p_3vc=NULL;
		p_3vmn=NULL;
	}

	int AEGLSLRenderUnit::StartInit(void)
	{
#if !defined(BUILD_GLES_RENDER)
		if(this->GetGLSLVersion()<120)
			return AE_ERR;
#endif

		if(!this->InitPrograms())
		{
			AEPrintLog("Failed to initialize shaders;");
			return AE_ERR;
		}

		if(!this->InitFramebuffers())
		{
			AEPrintLog("Failed to allocate framebuffers;");
			return AE_ERR; 
		}

		return AE_OK;
	}

	int AEGLSLRenderUnit::InitPrograms(void)
	{
		AEPrintLog("Initializing shaders:");
		int result=AE_OK;

		result&=this->Init3vcProgram();
		result&=this->Init3vmnProgram();
		// result&=this->InitPostPrograms();

		return result;
	}

	int AEGLSLRenderUnit::InitFramebuffers(void)
	{
		// glGenFramebuffers(1,&fbo_gbuffer);

		// g_color.reset(new _AEGLBuffer(width,height,GL_RGBA16F));
		// g_depth.reset(new _AEGLBuffer(width,height,GL_DEPTH_COMPONENT));
		// g_position.reset(new _AEGLBuffer(width,height,GL_RGBA16F));
		// g_normal.reset(new _AEGLBuffer(width,height,GL_RGB16F));

		// CheckError();

		// glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbo_gbuffer);

		// glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,g_depth->texture_unit,0);
		// glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,g_color->texture_unit,0);
		// glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT1,GL_TEXTURE_2D,g_position->texture_unit,0);
		// glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT2,GL_TEXTURE_2D,g_normal->texture_unit,0);

		// if(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
		// 	return AE_ERR;


		{	//post process framebuffer
			glGenFramebuffers(1,&fbo_post);

			post_color.reset(new _AEGLBuffer(width,height,GL_RGBA));
			CheckError();

			glBindFramebuffer(GL_FRAMEBUFFER,fbo_post);
			glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,post_color->texture_unit,0);

			if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
			{
				AEPrintLog("Post process buffer incomplete");
				return AE_ERR;
			}
		}

		{	//ONDS framebuffer
			glGenFramebuffers(1,&fbo_onds);

			char buf[128];
			sprintf(buf,"New framebuffers: %dx%d",width,height);
			AEPrintLog(buf);
			onds_depth.reset(new _AEGLBuffer(width,height,GL_DEPTH_COMPONENT));
			onds_color.reset(new _AEGLBuffer(width,height,GL_RGBA));
			CheckError();

			glBindFramebuffer(GL_FRAMEBUFFER,fbo_onds);
			glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,onds_depth->texture_unit,0);
			glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,onds_color->texture_unit,0);


			switch(glCheckFramebufferStatus(GL_FRAMEBUFFER))
			{
			case GL_FRAMEBUFFER_COMPLETE:
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				AEPrintLog("Framebuffer incomplete attachment");
				return AE_ERR;
			case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
				AEPrintLog("Framebuffer incomplete dimensions");
				return AE_ERR;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				AEPrintLog("Framebuffer missing attachment");
				return AE_ERR;
			case GL_FRAMEBUFFER_UNSUPPORTED:
				AEPrintLog("Framebuffer unsupported");
				return AE_ERR;
			}
		}

		return AE_OK;
	}

	void AEGLSLRenderUnit::BlitToScreen(void)
	{
#if !defined(BUILD_GLES_RENDER)
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);	//Bind default framebuffer to blit to screen

		glBindFramebuffer(GL_READ_FRAMEBUFFER,fbo_onds);//fbo_post);

		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glBlitFramebuffer(
			0,0,width,height,
			0,0,width,height,
			GL_COLOR_BUFFER_BIT,GL_NEAREST);

		if(use_MRT)
		{
			glBindFramebuffer(GL_READ_FRAMEBUFFER,fbo_gbuffer);

			glReadBuffer(GL_COLOR_ATTACHMENT0);
			glBlitFramebuffer(
				0,0,width,height,
				width*2/4+24,8,width*3/4+24,height/4+8,
				GL_COLOR_BUFFER_BIT,GL_NEAREST);

			glReadBuffer(GL_COLOR_ATTACHMENT1);
			glBlitFramebuffer(
				0,0,width,height,
				8,8,width/4+8,height/4+8,
				GL_COLOR_BUFFER_BIT,GL_NEAREST);

			glReadBuffer(GL_COLOR_ATTACHMENT2);
			glBlitFramebuffer(
				0,0,width,height,
				width/4+16,8,width*2/4+16,height/4+8,
				GL_COLOR_BUFFER_BIT,GL_NEAREST);
		}
#endif
	}

	void AEGLSLRenderUnit::PostProcess(AEObjectCamera *camera)
	{
		// if(use_MRT)
		// {
		// 	glEnable(GL_DEPTH_TEST);
		// 	glDisable(GL_STENCIL_TEST);
		// 	glDisable(GL_BLEND);

		// 	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbo_onds);
		// 	GLenum draw_bufs[]={GL_COLOR_ATTACHMENT0};
		// 	glDrawBuffers(1,draw_bufs);

		// 	p_post_invert->Use();

		// 	p_post_invert->BindData(
		// 		&this->sprite_mesh,		//mesh
		// 		g_color->texture_unit,	//texture
		// 		g_normal->texture_unit,
		// 		g_position->texture_unit,
		// 		0,
		// 		vec2f(width,height),	//texture size
		// 		vec4f(0,0,0,0),			//float data 0
		// 		vec4f(0,0,0,0),			//float data 1
		// 		&lighting_cache);

		// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sprite_mesh.idfce);

		// 	glDrawElements(GL_TRIANGLES,2*3,GL_UNSIGNED_INT,NULL);

		// 	p_post_invert->UnbindData();

		// 	glUseProgram(0);
		// }
	}

	int AEGLSLRenderUnit::ResizeFramebuffers(void)
	{
		// g_color->Resize(width,height);
		// g_depth->Resize(width,height);
		// g_position->Resize(width,height);
		// g_normal->Resize(width,height);

		post_color->Resize(width,height);

		onds_depth->Resize(width,height);
		onds_color->Resize(width,height);

		return this->CheckError();
	}

	int AEGLSLRenderUnit::Init(uint16_t width, uint16_t height)
	{
		int result=AEGLRenderUnit::Init(width,height,false);
		if(!result)
			return result;

		return this->StartInit();
	}

	void AEGLSLRenderUnit::Resize(uint16_t _width,uint16_t _height)
	{
		AEGLRenderUnit::Resize(_width,_height);

		this->ResizeFramebuffers();
	}

	// void AEGLSLRenderUnit::RenderObject(const AEObject * obj)
	// {
	// 	AEGLRenderUnit::RenderObject(obj);
	// 	//TODO maybe remove this override?
	// }

	unsigned int AEGLSLRenderUnit::GetGLSLVersion(void)
	{

		char ver[4];
		char *sver=(char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
		ver[0]=sver[0];
		ver[1]=sver[2];
		ver[2]=sver[3];
		ver[3]='\0';
		AEPrintLog("GLSL version:");
		AEPrintLog(sver);
		return atoi(ver);
	}

	int AEGLSLRenderUnit::Init3vcProgram(void)
	{
		this->p_3vc=new AEGLSLProgram3vc;//this->pmanager.NewProgram(); //FIXME Look here for proper creation of basic program
		AEGLSLShader *sv,*sf;
		sv=this->pmanager.NewShader(GL_VERTEX_SHADER);
		sf=this->pmanager.NewShader(GL_FRAGMENT_SHADER);

		char buf[1024];
		sprintf(buf,"Program: %d;\nShaders: %d,%d\n",p_3vc->id,sv->id,sf->id);
		AEPrintLog(buf);

		if(!sv||!sf)
		{
			AEPrintLog("Failed to create shaders");
			return AE_ERR;
		}

		const char *data=_3vc_v;//AEReadTextFileCml(AEGLSL_PATH_3VC_V);
		if(*data==AE_ERR) return AE_ERR;
		sv->ShaderData(&data,1,NULL);
		data=_3vc_f;//AEReadTextFileCml(AEGLSL_PATH_3VC_F);
		if(*data==AE_ERR) return AE_ERR;
		sf->ShaderData(&data,1,NULL);

		//Vertex shader
		sv->Compile();
		if(sv->GetCompileStatus()!=GL_TRUE)
		{
			AEPrintLog("err: v shader compilation failed:");
			AEPrintLog(sv->GetLog());
			return AE_ERR;
		}
		//Fragment shader
		sf->Compile();
		if(sf->GetCompileStatus()!=GL_TRUE)
		{
			AEPrintLog("err: f shader compilation failed:");
			AEPrintLog(sf->GetLog());
			return AE_ERR;
		}

		this->p_3vc->Attach(sv);
		this->p_3vc->Attach(sf);

		this->p_3vc->Link();
		if(this->p_3vc->GetLinkStatus()!=GL_TRUE)
		{
			AEPrintLog("err: program linkage failed:");
			AEPrintLog(this->p_3vc->GetLog());
			return AE_ERR;
		}

		this->p_3vc->GetShaderProperties();

		// char buf[1024];
		sprintf(buf,"Program: %d;\nShaders: %d,%d\n",p_3vc->id,sv->id,sf->id);
		AEPrintLog(buf);
		return AE_OK;
	}

	int AEGLSLRenderUnit::Init3vmnProgram(void)
	{
		this->p_3vmn=new AEGLSLProgram3vmn;//this->pmanager.NewProgram(); //FIXME Look here for proper creation of basic program
		AEGLSLShader *sv_mesh,*sf_mesh;
		sv_mesh=this->pmanager.NewShader(GL_VERTEX_SHADER);
		sf_mesh=this->pmanager.NewShader(GL_FRAGMENT_SHADER);

		const char *data=_3vc_v;//AEReadTextFileCml(AEGLSL_PATH_3VMN_V);
		if(*data==AE_ERR) return AE_ERR;
		sv_mesh->ShaderData(&data,1,NULL);
		data=_3vc_f;//AEReadTextFileCml(AEGLSL_PATH_3VMN_F);
		if(*data==AE_ERR) return AE_ERR;
		sf_mesh->ShaderData(&data,1,NULL);

		//Vertex shader
		sv_mesh->Compile();
		if(sv_mesh->GetCompileStatus()!=GL_TRUE)
		{
			AEPrintLog("err: v shader compilation failed:");
			AEPrintLog(sv_mesh->GetLog());
			return AE_ERR;
		}
		//Fragment shader
		sf_mesh->Compile();
		if(sf_mesh->GetCompileStatus()!=GL_TRUE)
		{
			AEPrintLog("err: f shader compilation failed:");
			AEPrintLog(sf_mesh->GetLog());
			return AE_ERR;
		}

		this->p_3vmn->Attach(sv_mesh);
		this->p_3vmn->Attach(sf_mesh);

		this->p_3vmn->Link();
		if(this->p_3vmn->GetLinkStatus()!=GL_TRUE)
		{
			AEPrintLog("err: program linkage failed:");
			AEPrintLog(this->p_3vmn->GetLog());
			return AE_ERR;
		}

		this->p_3vmn->GetShaderProperties();

		printf("Program: %d;\nShaders: %d,%d\n",p_3vmn->id,sv_mesh->id,sf_mesh->id);
		return AE_OK;
	}

	int AEGLSLRenderUnit::InitPostPrograms(void)
	{
		//TODO: Make program loading easier, too many duplicative code

		this->p_post_invert=new AEGLSLProgram2vsquarelight;//this->pmanager.NewProgram(); //FIXME Look here for proper creation of basic program
		AEGLSLShader *sv_mesh,*sf_mesh;
		sv_mesh=this->pmanager.NewShader(GL_VERTEX_SHADER);
		sf_mesh=this->pmanager.NewShader(GL_FRAGMENT_SHADER);

		const char **data=AEReadTextFileCml(AEGLSL_PATH_POST_INVERT_V);
		if(*data==AE_ERR) return AE_ERR;
		sv_mesh->ShaderData(data,1,NULL);
		data=AEReadTextFileCml(AEGLSL_PATH_POST_INVERT_F);
		if(*data==AE_ERR) return AE_ERR;
		sf_mesh->ShaderData(data,1,NULL);

		//Vertex shader
		sv_mesh->Compile();
		if(sv_mesh->GetCompileStatus()!=GL_TRUE)
		{
			AEPrintLog("err: v shader compilation failed:");
			AEPrintLog(sv_mesh->GetLog());
			return AE_ERR;
		}
		//Fragment shader
		sf_mesh->Compile();
		if(sf_mesh->GetCompileStatus()!=GL_TRUE)
		{
			AEPrintLog("err: f shader compilation failed:");
			AEPrintLog(sf_mesh->GetLog());
			return AE_ERR;
		}

		this->p_post_invert->Attach(sv_mesh);
		this->p_post_invert->Attach(sf_mesh);

		this->p_post_invert->Link();
		if(this->p_post_invert->GetLinkStatus()!=GL_TRUE)
		{
			AEPrintLog("err: program linkage failed:");
			AEPrintLog(this->p_post_invert->GetLog());
			return AE_ERR;
		}

		this->p_post_invert->GetShaderProperties();

		return AE_OK;
	}

	void AEGLSLRenderUnit::Render(AEObjectCamera *camera)
	{
		AEGLRenderUnit::Render(camera);

		PostProcess(camera);

		this->BlitToScreen();
	}

	AEGLSLRenderUnit::~AEGLSLRenderUnit()
	{
		if(this->p_3vc)
			delete p_3vc;
		if(this->p_3vmn)
			delete p_3vmn;

		glDeleteFramebuffers(1,&fbo_gbuffer);
		glDeleteFramebuffers(1,&fbo_post);
		glDeleteFramebuffers(1,&fbo_onds);
	}
}
