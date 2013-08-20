/*
 * AEGLSLRenderBG.cpp
 *
 *  Created on: Jul 2, 2012
 *      Author: klokik
 */

#include "AEGLHeader.h"
#include "AEGLSLRender.h"


namespace aengine
{
	void AEGLSLRenderUnit::RenderEmpty(AEObject * obj)
	{
		//glLoadMatrixf(cammatrix*obj->GetWorldMatrix());//obj->GetCameraMatrix());

		p_3vc->Use();

		AEColor white={1.0f,1.0f,1.0f,1.0f};
		p_3vc->BindData(obj->GetWorldMatrix(),cammatrix,prjmatrix,empty_mesh.idvtx,empty_mesh.idnrm,white);

		glDrawArrays(GL_LINES,0,6);

		p_3vc->UnbindData();

		glUseProgram(0);
	}

	void AEGLSLRenderUnit::RenderEmpties(void)
	{
		glBindFramebuffer(GL_FRAMEBUFFER,0);//fbo_onds);
		GLenum draw_bufs[]={GL_COLOR_ATTACHMENT0};
		// glDrawBuffers(1,draw_bufs);

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		for(size_t q=0;q<type_cache.empties.size();q++)
		{
			RenderEmpty(type_cache.empties[q]);
		}
	}
}
