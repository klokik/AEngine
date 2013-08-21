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
		//TODO: Make program loading easier, too many duplicative code

		this->p_blit=new AEGLSLProgram2vsquare;//this->pmanager.NewProgram(); //FIXME Look here for proper creation of basic program
		AEGLSLShader *sv_mesh,*sf_mesh;
		sv_mesh=this->pmanager.NewShader(GL_VERTEX_SHADER);
		sf_mesh=this->pmanager.NewShader(GL_FRAGMENT_SHADER);

		const char *data=_blit_v;
		if(*data==AE_ERR) return AE_ERR;
		sv_mesh->ShaderData(&data,1,NULL);
		data=_blit_f;
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

		this->p_blit->Attach(sv_mesh);
		this->p_blit->Attach(sf_mesh);

		this->p_blit->Link();
		if(this->p_blit->GetLinkStatus()!=GL_TRUE)
		{
			AEPrintLog("err: program linkage failed:");
			AEPrintLog(this->p_blit->GetLog());
			return AE_ERR;
		}

		this->p_blit->GetShaderProperties();

		return AE_OK;
	}

	AEGLESRenderUnit::~AEGLESRenderUnit(void)
	{
		// nop
	}
}