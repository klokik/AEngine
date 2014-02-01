/*
 * AEGLRenderParticles.cpp
 *
 *  Created on: Jan 04, 2014
 *      Author: klokik
 */

#include "AEGLHeader.h"
#include "AEGLRender.h"


namespace aengine
{
	#if !defined(AE_NEW_GL_CONTEXT)

	void AEGLRenderUnit::RenderParticleSystem(AEObjectParticleSystem *psystem)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glDisable(GL_TEXTURE_2D);

		glBindBuffer(GL_ARRAY_BUFFER,empty_mesh.idvtx);
		glVertexPointer(3,GL_FLOAT,0,0);
		glBindBuffer(GL_ARRAY_BUFFER,empty_mesh.idnrm);
		glColorPointer(4,GL_FLOAT,0,0);


		AEMatrix4f4 mtx=cammatrix*psystem->GetWorldMatrix();
		for(auto item:psystem->particles)
		{
			glLoadMatrixf(mtx*AEMatrix4f4().Translate(item.position).Scale(vec3f(0.1f,0.1f,0.1f)));
			glDrawArrays(GL_LINES,0,6);
		}

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	#else
	void AEGLRenderUnit::RenderParticleSystem(AEObjectParticleSystem *psystem) {}
	#endif

	void AEGLRenderUnit::RenderParticles(void)
	{
		for(auto *psystem:type_cache.particle_systems)
		{
			RenderParticleSystem(psystem);
		}
	}
}