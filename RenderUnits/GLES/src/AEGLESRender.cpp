#include "AEGLHeader.h"
#include "AEGLESRender.h"
#include "AEVectorMath.h"

#include "../resource/shader/builtin.h"


namespace aengine
{
	AEGLESRenderUnit::AEGLESRenderUnit(void)
	{
		this->use_MRT=false;
	}

	void AEGLESRenderUnit::BlitToScreen(void)
	{
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_BLEND);

		glBindFramebuffer(GL_FRAMEBUFFER,0);

		p_blit->Use();

		p_blit->BindData(
			&this->sprite_mesh,		//mesh
			onds_color->texture_unit,	//texture
			0,
			0,
			0,
			vec2f(width,height),	//texture size
			vec4f(0,0,0,0),			//float data 0
			vec4f(0,0,0,0)			//float data 1
		);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sprite_mesh.idfce);

		glDrawElements(GL_TRIANGLES,2*3,GL_UNSIGNED_INT,NULL);

		p_blit->UnbindData();

		glUseProgram(0);
	}

	int AEGLESRenderUnit::InitPrograms(void)
	{
		int result=AEGLSLRenderUnit::InitPrograms();

		result&=InitBlitProgram();

		return result;
	}

	int AEGLESRenderUnit::InitBlitProgram(void)
	{
		this->p_blit=new AEGLSLProgram2vsquare;

		return LoadCompileLink(*p_blit,_blit_v,_blit_f);
	}

	AEGLESRenderUnit::~AEGLESRenderUnit(void)
	{
		// nop
	}
}