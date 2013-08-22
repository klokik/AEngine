#include "AEGLSLRender.h"
#include "AEVectorMath.h"
#include "AEDebug.h"


namespace aengine
{
	void AEGLSLRenderUnit::RenderText(AEObjectText *obj)
	{
		AEFontBitmap *font;
		if(scene->fonts.fonts.size()>0)
			font=&scene->fonts.fonts.begin()->second;
		else
		{
			AEPrintLog("No font");
			return;
		}

		if(!obj->text.length())
			return;

		AEGLSLProgram3vm *p_btext=p_3vm;

		p_btext->Use();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sprite_mesh.idfce);

		float lambda=0;
		switch(obj->alignment)
		{
		case AE_CENTER: lambda=-0.5f; break;
		case AE_LEFT: lambda=0.0f; break;
		case AE_RIGHT: lambda=-1.0f; break;
		}

		AEMatrix4f4 m4_text;
		AEVector3f shift=obj->spacing*obj->text.length()*lambda;
		m4_text.Translate(shift);

		AEMaterial mat;
		mat.texture=font->texture;
		mat.diffuse.vec=vec4f(1.0f,1.0f,1.0f,1.0f);
		mat.shading=false;

		AEMatrix4f4 m4_ow(obj->GetWorldMatrix());

		for(char q:obj->text)
		{
			p_btext->BindData(
				m4_ow*m4_text,
				identity,
				orthomatrix,
				&this->sprite_mesh,
				&mat
			);

			glDrawElements(GL_TRIANGLES,3*2,GL_UNSIGNED_INT,NULL);

			m4_text.Translate(obj->spacing);
			// glMatrixMode(GL_TEXTURE);
			// glLoadIdentity();
			// glScalef(1.0f/font->width,1.0f/font->height,1.0f);
			// glTranslatef(q%font->width,q/font->height,0.0f);
			// glDrawElements(GL_TRIANGLES,3*2,GL_UNSIGNED_INT,NULL);
			// glMatrixMode(GL_MODELVIEW);
			// glTranslatef(obj->spacing.X,obj->spacing.Y,obj->spacing.Z);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
		p_btext->UnbindData();
		glUseProgram(0);
	}

	void AEGLSLRenderUnit::RenderTexts(void)
	{
		glBindFramebuffer(GL_FRAMEBUFFER,fbo_onds);

		for(size_t q=0;q<type_cache.texts.size();q++)
		{
			RenderText(type_cache.texts[q]);
		}
	}
}