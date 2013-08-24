/*
 * AEGLSLRenderSprite.cpp
 *
 *  Created on: Feb 12, 2012
 *      Author: klokik
 */

#include "AEGLHeader.h"
#include "AEGLSLRender.h"


namespace aengine
{
	void AEGLSLRenderUnit::RenderSpritesPersp(void)
	{
		glBindFramebuffer(GL_FRAMEBUFFER,fbo_onds);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_BLEND);

		p_3vm->Use();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sprite_mesh.idfce);

		for(AEObjectSprite *obj:type_cache.sprites_persp)
		{
			p_3vm->BindData(
				obj->GetWorldMatrix(),
				cammatrix,
				prjmatrix,
				&this->sprite_mesh,
				obj->material
			);

			glDrawElements(GL_TRIANGLES,2*3,GL_UNSIGNED_INT,NULL);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

		p_3vm->UnbindData();

		glUseProgram(0);
	}

	void AEGLSLRenderUnit::RenderSpritesOrtho(void)
	{
		glBindFramebuffer(GL_FRAMEBUFFER,fbo_onds);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_BLEND);

		p_3vm->Use();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sprite_mesh.idfce);

		for(AEObjectSprite *obj:type_cache.sprites_ortho)
		{
			p_3vm->BindData(
				obj->GetWorldMatrix(),
				identity,
				orthomatrix,
				&this->sprite_mesh,
				obj->material
			);

			glDrawElements(GL_TRIANGLES,2*3,GL_UNSIGNED_INT,NULL);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

		p_3vm->UnbindData();

		glUseProgram(0);
	}
}

