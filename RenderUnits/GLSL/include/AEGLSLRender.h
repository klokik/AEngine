/*
 * AEGLSLRender.h
 *
 *  Created on: Jan 31, 2012
 *      Author: klokik
 */

#ifndef AEGLSLRENDER_H_
#define AEGLSLRENDER_H_

#include "AEGLRender.h"
#include "AEGLSLProgramManager.h"
#include "AEGLSLDerivedPrograms.h"

#include "AEGLSLFilter.h"


namespace aengine
{
	class AEGLSLBasicProgram;

	class AEGLSLRenderUnit: public AEGLRenderUnit
	{
	protected:
		bool use_MRT;	// use multiple render targets
		bool use_gbuffer;
		bool use_postprocessing;

		AEGLSLProgramManager pmanager;

		AEGLSLProgram3vc	*p_3vc;
		AEGLSLProgram3vm	*p_3vm;
		AEGLSLProgram3vmn	*p_3vmn;
		AEGLSLProgram3vmnl	*p_3vmnl;

		AEGLSLProgram2vsquarelight *p_post_combine_gbuffer;

		//framebuffer object to render scene into it
		// uint fbo_main;

		//framebuffer used for deferred rendering
		uint fbo_gbuffer;
		AEGLBuffer g_color;
		AEGLBuffer g_depth;
		AEGLBuffer g_position;
		AEGLBuffer g_normal;

		//framebuffer object for rendering texture to texture per pixel
		uint fbo_post;
		AEGLBuffer post_color;

		//Framebuffer for Other, Not Deferred Stuff (ONDS)
		uint fbo_onds;
		AEGLBuffer onds_depth;
		AEGLBuffer onds_color;

		//AEGLSLFilter filter;
		//AEGLBuffer color_output;

		virtual int StartInit(void);
		virtual int InitPrograms(void);
		virtual int InitFramebuffers(void);
		virtual int ResizeFramebuffers(void);

		virtual void BlitToScreen(void);
		virtual void PostProcess(AEObjectCamera *camera);

		int LoadCompileLink(AEGLSLProgram &prog,std::string shader_v,std::string shader_f);
		// virtual int Init3vcProgram(void);
		// virtual int Init3vmnProgram(void);
		virtual int InitPostPrograms(void);

		virtual unsigned int GetGLSLVersion(void);

		virtual void ClearFramebuffers(void);

		virtual void RenderEmpty(AEObject * obj);
		virtual void RenderMesh(AEObjectMesh * obj);
		virtual void RenderParticleSystem(AEObjectParticleSystem *psystem);

		virtual void RenderText(AEObjectText *obj) override;

		virtual void RenderMeshes(void) override;
		virtual void RenderEmpties(void) override;

		virtual void RenderSpritesPersp(void) override;
		virtual void RenderSpritesOrtho(void) override;

		virtual void RenderTexts(void) override;
	//	virtual void RenderSpritePersp(const AESpriteObject & obj);
	//	virtual void RenderSpritesOrtho(void);

	public:
		AEGLSLRenderUnit();

		virtual int Init(uint16_t _width,uint16_t _height);
		virtual void Resize(uint16_t _width,uint16_t _height);
		//virtual void RenderObject(const AEObject * obj);

		virtual void Render(AEObjectCamera *camera);

		virtual ~AEGLSLRenderUnit();
	};
}

#endif /* AEGLSLRENDER_H_ */
