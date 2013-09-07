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
		AEVector3f shift=obj->spacing*(obj->text.length()-1)*lambda;
		m4_text.Translate(shift);

		AEMaterial mat;
		mat.texture=font->texture;
		mat.diffuse.vec=vec4f(1.0f,1.0f,1.0f,1.0f);
		mat.transparent=true;
		mat.shading=false;

		AEMatrix4f4 m4_ow(obj->GetWorldMatrix());

		AETexCoord *orig_tcr=sprite_mesh.tcr;

		AETexCoord tcr[4];

		sprite_mesh.tcr=tcr;

		for(char i:obj->text)
		{
			unsigned char q = static_cast<unsigned char>(i);

			float d_hx = 1/16.0f;
			Vec3f lc = vec3f(q%font->width,q/font->height,0.0f)*d_hx;

			tcr[0].vec=lc+vec3f(0.0f,0.0f,0.0f);
			tcr[1].vec=lc+vec3f(d_hx,0.0f,0.0f);
			tcr[2].vec=lc+vec3f(d_hx,d_hx,0.0f);
			tcr[3].vec=lc+vec3f(0.0f,d_hx,0.0f);
			sprite_mesh.Invalidate(AE_UPDATE_TEXCOORD);
			AEGLSLRenderUnit::UpdateMeshBuffers(&sprite_mesh);

			p_btext->BindData(
				m4_ow*m4_text,
				identity,
				orthomatrix,
				&this->sprite_mesh,
				&mat
			);

			glDrawElements(GL_TRIANGLES,3*2,GL_UNSIGNED_INT,NULL);

			m4_text.Translate(obj->spacing);
		}

		p_btext->UnbindData();

		glUseProgram(0);

		sprite_mesh.tcr=orig_tcr;
		sprite_mesh.Invalidate(AE_UPDATE_TEXCOORD);
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