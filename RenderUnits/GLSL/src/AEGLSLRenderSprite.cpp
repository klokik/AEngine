/*
 * AEGLSLRenderSprite.cpp
 *
 *  Created on: Feb 12, 2012
 *      Author: klokik
 */

#include <algorithm>

#include "AEGLHeader.h"
#include "AEGLSLRender.h"
#include "AEVectorMath.h"
#include "AEDebug.h"


namespace aengine
{
	void AEGLSLRenderUnit::RenderSpritesPersp(void)
	{
		if(sprite_mesh.IsInvalid())
			AEGLRenderUnit::UpdateMeshBuffers(&sprite_mesh);

		glBindFramebuffer(GL_FRAMEBUFFER,fbo_onds);

		// glDisable(GL_DEPTH_TEST);

		p_3vm->Use();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sprite_mesh.idfce);

		// sort sprites by distance from camera and draw in reverse order
		class d_sorter
		{
		protected:
			AEObject *obj;
		public:
			d_sorter(AEObject *cam)
			{
				obj = cam;
			}

			bool operator()(AEObjectSprite *a,AEObjectSprite *b)
			{
				float l_a = Length(a->GetAbsPosition()-obj->GetAbsPosition());
				float l_b = Length(b->GetAbsPosition()-obj->GetAbsPosition());
				return l_a>l_b;
			}
		};

		d_sorter sorter_inst(curCam);
		std::sort(type_cache.sprites_persp.begin(),type_cache.sprites_persp.end(),sorter_inst);

		AEMatrix4f4 mtx;
		for(AEObjectSprite *obj:type_cache.sprites_persp)
		{
			//TODO: Far not the best solution
			// <hell>
			AEMatrix4f4 A=cammatrix*obj->GetWorldMatrix();
			Vec3f scale=obj->GetAbsScale();
			A[0]=scale.X;
			A[5]=scale.Y;
			A[10]=scale.Z;
			A[15]=1.0f;
			float mtx_f[]={
				A[ 0],    0,    0,    0,
				    0,A[ 5],    0,    0,
				    0,    0,A[10],    0,
				A[12],A[13],A[14],A[15]
			};

			mtx=mtx_f;

			p_3vm->BindData(
				mtx,
				identity,
				prjmatrix,
				&this->sprite_mesh,
				obj->material
			);
			// </hell>

			glDrawElements(GL_TRIANGLES,2*3,GL_UNSIGNED_INT,NULL);
		}

		p_3vm->UnbindData();
		CheckError();

		glUseProgram(0);
	}

	void AEGLSLRenderUnit::RenderSpritesOrtho(void)
	{
		if(sprite_mesh.IsInvalid())
			AEGLRenderUnit::UpdateMeshBuffers(&sprite_mesh);

		glBindFramebuffer(GL_FRAMEBUFFER,fbo_onds);

		// glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);

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

		p_3vm->UnbindData();

		glUseProgram(0);
	}
}

