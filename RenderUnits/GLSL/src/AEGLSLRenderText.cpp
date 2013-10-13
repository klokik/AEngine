#include <algorithm>
#include <sstream>

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

		float char_lambda=0;
		float line_lambda=-0.5f;
		switch(obj->char_alignment)
		{
		case AE_CENTER: char_lambda=-0.5f; break;
		case AE_LEFT: char_lambda=0.0f; break;
		case AE_RIGHT: char_lambda=-1.0f; break;
		}
		// TODO: align lines the same way as chars

		AEMatrix4f4 m4_text;
		size_t lines=std::count(obj->text.begin(),obj->text.end(),'\n');
		AEVector3f shift=
//			obj->char_spacing*(obj->text.length()-1)*char_lambda+	// chars
			obj->line_spacing*(lines)*line_lambda;				// lines
		m4_text=m4_text.Translate(shift);

		AEMaterial mat;
		mat.texture=font->texture;
		mat.diffuse.vec=vec4f(1.0f,1.0f,1.0f,1.0f);
		mat.transparent=true;
		mat.shading=false;

		AEMatrix4f4 m4_ow(obj->GetWorldMatrix());

		AETexCoord *orig_tcr=sprite_mesh.tcr;

		AETexCoord tcr[4];

		sprite_mesh.tcr=tcr;

		std::stringstream str_text(obj->text);
		std::string line;
		size_t line_num = 0;
		while(std::getline(str_text,line))
		{
			Vec3f shift=
				obj->char_spacing*(line.length()-1)*char_lambda+
				obj->line_spacing*(lines*line_lambda+line_num);
			m4_text=AEMatrix4f4().Translate(shift);
			line_num++;

			for(char i:line)
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

				m4_text=m4_text.Translate(obj->char_spacing);
			}
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