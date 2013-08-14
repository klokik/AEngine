/*
 * AEGLRenderMesh.cpp
 *
 *  Created on: Oct 8, 2011
 *      Author: klokik
 */

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

#include <GL/gl.h>

#include "AEGLRender.h"


namespace aengine
{
	void AEGLRenderUnit::RenderMesh(AEObjectMesh * obj)
	{
		if(!obj->mesh) return;

		bool cTexCoord=false;

		glColor3f(1.0f,1.0f,1.0f);

		cTexCoord=this->ApplyMaterial(obj->material);
		if(cTexCoord&&obj->mesh->tcrcount)
		{
			glBindBuffer(GL_ARRAY_BUFFER,obj->mesh->idtcr);
			glTexCoordPointer(3,GL_FLOAT,sizeof(AETexCoord),0);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnable(GL_TEXTURE_2D);
		}


		glBindBuffer(GL_ARRAY_BUFFER,obj->mesh->idvtx);
		glVertexPointer(3,GL_FLOAT,sizeof(AEVertexf),0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,obj->mesh->idfce);

		glEnableClientState(GL_VERTEX_ARRAY);

		if(obj->projection==AE_ORTHOGRAPHIC)
		{
			this->Set2DMode();
			this->SetFixedProjectionMatrix();
			glLoadMatrixf(obj->GetWorldMatrix().ToArray());
		}
		else
			glLoadMatrixf(cammatrix*obj->GetWorldMatrix());

		glDrawElements(GL_TRIANGLES,obj->mesh->fcecount*3,GL_UNSIGNED_INT,0);

		if(obj->projection==AE_ORTHOGRAPHIC)
		{
			this->PopMode();
			this->SetFixedProjectionMatrix();
		}


		if(cTexCoord)
		{
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisable(GL_TEXTURE_2D);
		}

		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void AEGLRenderUnit::RenderMeshes(void)
	{
		for(size_t q=0;q<type_cache.meshes.size();q++)
		{
			RenderMesh(dynamic_cast<AEObjectMesh*>(type_cache.meshes[q]));
		}
	}
}
