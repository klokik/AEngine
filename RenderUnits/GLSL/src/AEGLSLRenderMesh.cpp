/*
 * AEGLSLRenderMesh.cpp
 *
 *  Created on: Feb 1, 2012
 *      Author: klokik
 */

#include "AEGLHeader.h"
#include "AEGLSLRender.h"
#include "AEObjectMesh.h"


namespace aengine
{
	void AEGLSLRenderUnit::RenderMesh(AEObjectMesh * obj)
	{
		if(!obj->mesh) return;

		if(obj->mesh->IsInvalid())
			AEGLRenderUnit::UpdateMeshBuffers(obj->mesh);

		p_3vmn->Use();

		p_3vmn->BindData(
				obj->GetWorldMatrix(),
				obj->projection==AE_ORTHOGRAPHIC?identity:cammatrix,
				prjmatrix,
				obj->mesh,
				obj->material);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,obj->mesh->idfce);

		glDrawElements(GL_TRIANGLES,obj->mesh->fcecount*3,GL_UNSIGNED_INT,0);

		// p_3vmn->UnbindData();

		glUseProgram(0);
	}

	void AEGLSLRenderUnit::RenderMeshes(void)
	{
		glBindFramebuffer(GL_FRAMEBUFFER,fbo_onds);//fbo_gbuffer);
		// GLenum draw_bufs[]={GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2};
		// glDrawBuffers(3,draw_bufs);

		glEnable(GL_DEPTH_TEST);

		for(size_t q=0;q<type_cache.meshes.size();q++)
		{
			RenderMesh(dynamic_cast<AEObjectMesh*>(type_cache.meshes[q]));
		}

		//TODO: Apply lighting
	}
}
