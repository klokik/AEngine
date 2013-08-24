/*
 * AEGLRenderSprite.cpp
 *
 *  Created on: Dec 4, 2011
 *      Author: klokik
 */

#include <math.h>

#include "AEGLHeader.h"
#include "AEGLRender.h"


#if !defined(AE_NEW_GL_CONTEXT)
namespace aengine
{
	const float rad=57.29577951f;

	void AEGLRenderUnit::RenderSpritesPersp(void)
	{
		glBindBuffer(GL_ARRAY_BUFFER,sprite_mesh.idvtx);
		glVertexPointer(3,GL_FLOAT,0,NULL);

		glBindBuffer(GL_ARRAY_BUFFER,sprite_mesh.idnrm);
		glNormalPointer(GL_FLOAT,0,NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sprite_mesh.idfce);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		for(AEObjectSprite *obj:type_cache.sprites_persp)
		{
			glLoadMatrixf(cammatrix*obj->GetWorldMatrix());

			if(ApplyMaterial(obj->material))
			{
				glEnable(GL_TEXTURE_2D);
				glDrawElements(GL_TRIANGLES,2*3,GL_UNSIGNED_INT,NULL);
				glDisable(GL_TEXTURE_2D);

				if(obj->tcr_usedefault)
				{
					glBindBuffer(GL_ARRAY_BUFFER,sprite_mesh.idtcr);
					glTexCoordPointer(3,GL_FLOAT,0,NULL);
				}
				else
				{
					//FIXME USE BUFFERS HERE
					glTexCoordPointer(3,GL_FLOAT,0,obj->tcr);
				}
			}
			else
				glDrawElements(GL_TRIANGLES,2*3,GL_UNSIGNED_INT,NULL);
		}

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	void AEGLRenderUnit::RenderSpritesOrtho(void)
	{
		glBindBuffer(GL_ARRAY_BUFFER,sprite_mesh.idvtx);
		glVertexPointer(3,GL_FLOAT,0,NULL);

		glBindBuffer(GL_ARRAY_BUFFER,sprite_mesh.idtcr);
		glTexCoordPointer(3,GL_FLOAT,0,NULL);

		glBindBuffer(GL_ARRAY_BUFFER,sprite_mesh.idnrm);
		glNormalPointer(GL_FLOAT,0,NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sprite_mesh.idfce);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		// this->Set2DMode();
		this->SetFixedProjectionMatrix();

		for(AEObjectSprite *obj:type_cache.sprites_ortho)
		{
			glLoadMatrixf(obj->GetWorldMatrix().ToArray());

			if(ApplyMaterial(obj->material))
			{
				glEnable(GL_TEXTURE_2D);
				glDrawElements(GL_TRIANGLES,2*3,GL_UNSIGNED_INT,NULL);
				glDisable(GL_TEXTURE_2D);
			}
			else
				glDrawElements(GL_TRIANGLES,2*3,GL_UNSIGNED_INT,NULL);
		}

		// this->PopMode();
		this->SetFixedProjectionMatrix();

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}
}
#else
namespace aengine
{
	void AEGLRenderUnit::RenderSpritesPersp(void) {};
	void AEGLRenderUnit::RenderSpritesOrtho(void) {};
}
#endif /* !AE_NEW_GL_CONTEXT */
