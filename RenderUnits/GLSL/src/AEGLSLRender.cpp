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
#include "AEResourceManager.h"

// Temporary
#include "../resource/shader/builtin.h"


namespace aengine
{
	AEGLSLRenderUnit::AEGLSLRenderUnit()
	{
		use_MRT=true;
		use_gbuffer=true;
		use_postprocessing=false;

		p_3vc=nullptr;
		p_3vmn=nullptr;
		p_3vmnl=nullptr;
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

		p_3vc=new AEGLSLProgram3vc;
		p_3vmn=new AEGLSLProgram3vmn;
		p_3vmnl=new AEGLSLProgram3vmnl;

		result&=LoadCompileLink(*p_3vc,
			AEResourceManager::LoadString(AEGLSL_PATH_3VC_V),
			AEResourceManager::LoadString(AEGLSL_PATH_3VC_F));

		result&=LoadCompileLink(*p_3vmn,
			AEResourceManager::LoadString(AEGLSL_PATH_3VMN_V),
			AEResourceManager::LoadString(AEGLSL_PATH_3VMN_F));

		result&=LoadCompileLink(*p_3vmnl,
			AEResourceManager::LoadString(AEGLSL_PATH_3VMNL_V),
			AEResourceManager::LoadString(AEGLSL_PATH_3VMNL_F));

		p_3vm=static_cast<AEGLSLProgram3vm*>(p_3vmn);

		result&=this->InitPostPrograms();

		return result;
	}

	int AEGLSLRenderUnit::InitFramebuffers(void)
	{
		if(use_gbuffer)
		{
			glGenFramebuffers(1,&fbo_gbuffer);

			// TODO: 16F
			g_color.reset(new _AEGLBuffer(width,height,GL_RGBA));
			g_depth.reset(new _AEGLBuffer(width,height,GL_DEPTH_COMPONENT));
			g_position.reset(new _AEGLBuffer(width,height,GL_RGBA));
			g_normal.reset(new _AEGLBuffer(width,height,GL_RGB));

			CheckError();

			glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbo_gbuffer);

			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,g_depth->texture_unit,0);
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,g_color->texture_unit,0);
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT1,GL_TEXTURE_2D,g_position->texture_unit,0);
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT2,GL_TEXTURE_2D,g_normal->texture_unit,0);

			if(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
			{
				AEPrintLog("G_buffer incomplete");
				return AE_ERR;
			}
		}

		if(use_postprocessing)
		{
			glGenFramebuffers(1,&fbo_post);

			post_color.reset(new _AEGLBuffer(width,height,GL_RGBA));
			CheckError();

			glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbo_post);
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,post_color->texture_unit,0);

			if(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
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

			glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbo_onds);
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,onds_depth->texture_unit,0);
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,onds_color->texture_unit,0);


			switch(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER))
			{
			case GL_FRAMEBUFFER_COMPLETE:
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				AEPrintLog("Framebuffer incomplete attachment");
				return AE_ERR;
			// case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
			// 	AEPrintLog("Framebuffer incomplete dimensions");
			// 	return AE_ERR;
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
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);	// default framebuffer

		if(use_postprocessing)
			glBindFramebuffer(GL_READ_FRAMEBUFFER,fbo_post);
		else
			glBindFramebuffer(GL_READ_FRAMEBUFFER,fbo_onds);

		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glBlitFramebuffer(
			0,0,width,height,
			0,0,width,height,
			GL_COLOR_BUFFER_BIT,GL_NEAREST);

		if(use_MRT&&use_gbuffer)
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
		if(use_MRT&&use_gbuffer)
		{
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_STENCIL_TEST);
			glDisable(GL_BLEND);

			glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbo_onds);
			GLenum draw_bufs[]={GL_COLOR_ATTACHMENT0};
			glDrawBuffers(1,draw_bufs);

			p_post_combine_gbuffer->Use();

			p_post_combine_gbuffer->BindData(
				&this->sprite_mesh,		//mesh
				g_color->texture_unit,	//texture
				g_normal->texture_unit,
				g_position->texture_unit,
				0,
				vec2f(width,height),	//texture size
				vec4f(0,0,0,0),			//float data 0
				vec4f(0,0,0,0),			//float data 1
				&lighting_cache);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sprite_mesh.idfce);

			glDrawElements(GL_TRIANGLES,2*3,GL_UNSIGNED_INT,NULL);

			p_post_combine_gbuffer->UnbindData();
		}
	}

	int AEGLSLRenderUnit::ResizeFramebuffers(void)
	{
		if(use_gbuffer)
		{
			g_color->Resize(width,height);
			g_depth->Resize(width,height);
			g_position->Resize(width,height);
			g_normal->Resize(width,height);
		}

		if(use_postprocessing)
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

	int AEGLSLRenderUnit::LoadCompileLink(AEGLSLProgram &prog,std::string shader_v,std::string shader_f)
	{
		AEGLSLShader sv(GL_VERTEX_SHADER);
		AEGLSLShader sf(GL_FRAGMENT_SHADER);

		sv.ShaderData(shader_v);
		sf.ShaderData(shader_f);

		if(!sv.Compile()||!sf.Compile())
			return AE_ERR;

		prog.Attach(sv);
		prog.Attach(sf);

		int lresult=prog.Link();

		prog.Detach(sv);
		prog.Detach(sf);

		if(!lresult)
			return AE_ERR;

		prog.GetShaderProperties();

		return this->CheckError();
	}

	int AEGLSLRenderUnit::InitPostPrograms(void)
	{
		int result=AE_OK;

		p_post_combine_gbuffer=new AEGLSLProgram2vsquarelight;

		result&=LoadCompileLink(*p_post_combine_gbuffer,
			AEResourceManager::LoadString(AEGLSL_PATH_POST_COMBINE_GBUFFER_V),
			AEResourceManager::LoadString(AEGLSL_PATH_POST_COMBINE_GBUFFER_F));

		return result;
	}

	void AEGLSLRenderUnit::ClearFramebuffers(void)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbo_onds);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		if(use_gbuffer)
		{
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbo_gbuffer);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		}

		if(use_postprocessing)
		{
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbo_post);
			glClear(GL_COLOR_BUFFER_BIT);
		}
	}

	void AEGLSLRenderUnit::Render(AEObjectCamera *camera)
	{
		ClearFramebuffers();

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
