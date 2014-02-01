/*
 * AEGLSLRenderParticles.cpp
 *
 *  Created on: Jan 05, 2014
 *      Author: klokik
 */

#include "AEGLHeader.h"
#include "AEGLSLRender.h"


namespace aengine
{
	void AEGLSLRenderUnit::RenderParticleSystem(AEObjectParticleSystem *psystem)
	{
		if(sprite_mesh.IsInvalid())
			AEGLRenderUnit::UpdateMeshBuffers(&sprite_mesh);

		glBindFramebuffer(GL_FRAMEBUFFER,fbo_onds);

		p_3vm->Use();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sprite_mesh.idfce);

		p_3vm->BindData(
			psystem->GetWorldMatrix(),
			psystem->projection==AE_ORTHOGRAPHIC?identity:cammatrix,
			prjmatrix,
			&this->sprite_mesh,
			psystem->material
		);

		for(auto item:psystem->particles)
		{
			glUniformMatrix4fv(p_3vm->u_object_matrix.id,1,false,
				psystem->GetWorldMatrix().Translate(item.position).Scale(item.size));
			glDrawElements(GL_TRIANGLES,2*3,GL_UNSIGNED_INT,NULL);
		}

		p_3vm->UnbindData();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sprite_mesh.idfce);
	}
}