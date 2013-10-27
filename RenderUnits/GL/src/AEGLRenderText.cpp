/*
 * AEGLRenderText.cpp
 *
 *  Created on: Oct 13, 2012
 *      Author: klokik
 */

#include "AEGLHeader.h"
#include "AEGLRender.h"
#include "AEVectorMath.h"


#if !defined(AE_NEW_GL_CONTEXT)
namespace aengine
{
	void AEGLRenderUnit::RenderText(AEObjectText *obj)
	{
		AEFontBitmap *font;
		if(scene->fonts.fonts.size()>0)
			font=&scene->fonts.fonts.begin()->second;
		else
			return;

		if(!obj->text.length())
			return;

		if(obj->projection==AE_ORTHOGRAPHIC)
		{
			this->Set2DMode();
			this->SetFixedProjectionMatrix();
			glLoadMatrixf(obj->GetWorldMatrix().ToArray());
		}
		else
			glLoadMatrixf(cammatrix*obj->GetWorldMatrix());

		glBindBuffer(GL_ARRAY_BUFFER,sprite_mesh.idvtx);
		glVertexPointer(3,GL_FLOAT,0,NULL);

		glBindBuffer(GL_ARRAY_BUFFER,sprite_mesh.idtcr);
		glTexCoordPointer(3,GL_FLOAT,0,NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sprite_mesh.idfce);

		glColor3f(1.0f,1.0f,1.0f);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		float lambda=0;
		switch(obj->char_alignment)
		{
		case AE_CENTER: lambda=-0.5f; break;
		case AE_LEFT: lambda=0.0f; break;
		case AE_RIGHT: lambda=-1.0f; break;
		}

		AEVector3f shift=obj->char_spacing*obj->text.length()*lambda;
		glTranslatef(shift.X,shift.Y,shift.Z);

		glBindTexture(GL_TEXTURE_2D,font->texture->id);
		glEnable(GL_TEXTURE_2D);
		for(char q:obj->text)
		{
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glScalef(1.0f/font->width,1.0f/font->height,1.0f);
			glTranslatef(q%font->width,q/font->height,0.0f);
			glDrawElements(GL_TRIANGLES,3*2,GL_UNSIGNED_INT,NULL);
			glMatrixMode(GL_MODELVIEW);
			glTranslatef(obj->char_spacing.X,obj->char_spacing.Y,obj->char_spacing.Z);
		}
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		if(obj->projection==AE_ORTHOGRAPHIC)
		{
			this->PopMode();
			this->SetFixedProjectionMatrix();
		}
	}

	void AEGLRenderUnit::RenderTexts(void)
	{
		for(size_t q=0;q<type_cache.texts.size();q++)
		{
			RenderText(type_cache.texts[q]);
		}
	}
}
#else
namespace aengine
{
	void AEGLRenderUnit::RenderText(AEObjectText *obj) {}
	void AEGLRenderUnit::RenderTexts(void) {}
}
#endif /* !AE_NEW_GL_CONTEXT */
