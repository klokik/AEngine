/*
 * AEGLSLDerivedPrograms.cpp
 *
 *  Created on: Mar 14, 2012
 *      Author: klokik
 */

#include "AEGLHeader.h"
#include "AEGLSLDerivedPrograms.h"


namespace aengine
{
	AEGLSLProgram3vc::AEGLSLProgram3vc(void)
	{
		u_object_matrix.name=		"u_object_matrix";		u_object_matrix.type=GL_FLOAT_MAT4;
		u_camera_matrix.name=		"u_camera_matrix";		u_camera_matrix.type=GL_FLOAT_MAT4;
		u_projection_matrix.name=	"u_projection_matrix";	u_projection_matrix.type=GL_FLOAT_MAT4;
		a_vertex.name	="a_vertex";		a_vertex.type=GL_FLOAT_VEC3;
		a_color.name	="a_color";			a_color.type =GL_FLOAT_VEC4;
		u_color.name	="u_color";			u_color.type =GL_FLOAT_VEC4;
	}

	void AEGLSLProgram3vc::GetShaderProperties(void)
	{
		this->GetAttribute(a_vertex);
		this->GetAttribute(a_color);

		this->GetUniform(u_object_matrix);
		this->GetUniform(u_camera_matrix);
		this->GetUniform(u_projection_matrix);
		this->GetUniform(u_color);

		printf("%d,%d,%d\n",a_vertex.id,a_color.id,u_color.id);
	}

	bool AEGLSLProgram3vc::BindData(
		const AEMatrix4f4 &object,
		const AEMatrix4f4 &camera,
		const AEMatrix4f4 &projection,
		int vbo,
		int cbo,
		AEColor &color)
	{
		if(a_vertex.id!=-1&&vbo)
		{
			glBindBuffer(GL_ARRAY_BUFFER,vbo);
			glVertexAttribPointer(a_vertex.id,3,GL_FLOAT,GL_FALSE,sizeof(AEVertexf),0);
			glEnableVertexAttribArray(a_vertex.id);
		}
		else
			return false;	//There's no sense to pass other data if we haven't vertices

		if(a_color.id!=-1&&cbo)
		{
			glBindBuffer(GL_ARRAY_BUFFER,cbo);
			glVertexAttribPointer(a_color.id,4,GL_FLOAT,false,0,0);
			glEnableVertexAttribArray(a_color.id);
		}

		glBindBuffer(GL_ARRAY_BUFFER,0);

		glUniformMatrix4fv(u_object_matrix.id,1,false,object.ToArray());
		glUniformMatrix4fv(u_camera_matrix.id,1,false,camera.ToArray());
		glUniformMatrix4fv(u_projection_matrix.id,1,false,projection.ToArray());

		glUniform4f(u_color.id,color.R,color.G,color.B,color.A);

		return true;
	}

	void AEGLSLProgram3vc::UnbindData(void)
	{
		int result;
		glGetVertexAttribiv(a_vertex.id,GL_VERTEX_ATTRIB_ARRAY_ENABLED,&result);
		if(result)
			glDisableVertexAttribArray(a_vertex.id);

		glGetVertexAttribiv(a_color.id,GL_VERTEX_ATTRIB_ARRAY_ENABLED,&result);
		if(result)
			glDisableVertexAttribArray(a_color.id);
	}

	AEGLSLProgram3vm::AEGLSLProgram3vm(void)
	{
		u_istextured0.name="u_istextured0";	u_istextured0.type=GL_INT;
		u_texture0.name	="u_texture0";		//u_texture0.type=GL_UNSIGNED_INT_SAMPLER_2D;
		a_texcoord0.name="a_texcoord0";		a_texcoord0.type=GL_FLOAT_VEC3;

		texturing=true;
	}

	void AEGLSLProgram3vm::GetShaderProperties(void)
	{
		AEGLSLProgram3vc::GetShaderProperties();
		this->GetUniform(u_istextured0);
		this->GetUniform(u_texture0);
		this->GetAttribute(a_texcoord0);
	}

	bool AEGLSLProgram3vm::BindData(
			const AEMatrix4f4 &object,
			const AEMatrix4f4 &camera,
			const AEMatrix4f4 &projection,
			AEMesh * mesh,
			AEMaterial *mat)
	{
		AEColor col;
		if(mat==NULL)
		{
			col={1.0f,1.0f,1.0f,1.0f};
		}
		else
			col=mat->diffuse;

		if(mesh)
		{
			if(!AEGLSLProgram3vc::BindData(object,camera,projection,mesh->idvtx,-1,col))
				return false;
		}
		else
			return false;

		if(a_texcoord0.id!=-1&&mesh->idtcr&&mat&&mat->texture)
		{
			if(mat->texture->size>0)
			{
				// it's useless while using shaders
				// glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,mat->texture->id);

				glBindBuffer(GL_ARRAY_BUFFER,mesh->idtcr);//TODO For 3D tex. coords look here
				glVertexAttribPointer(a_texcoord0.id,2,GL_FLOAT,GL_FALSE,sizeof(AETexCoord),0);
				glEnableVertexAttribArray(a_texcoord0.id);

				glUniform1i(u_texture0.id,0);
				glUniform1i(u_istextured0.id,this->texturing);

				return true;
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER,0);

		glUniform1i(u_istextured0.id,0);
		return true;	//Return true if geometry is OK
	}

	void AEGLSLProgram3vm::UnbindData(void)
	{
		AEGLSLProgram3vc::UnbindData();

		int result;
		glGetVertexAttribiv(a_texcoord0.id,GL_VERTEX_ATTRIB_ARRAY_ENABLED,&result);
		if(result)
			glDisableVertexAttribArray(a_texcoord0.id);
	}

	AEGLSLProgram3vmn::AEGLSLProgram3vmn(void)
	{
		a_normal.name = "a_normal";		a_normal.type=GL_FLOAT_VEC3;
	}

	void AEGLSLProgram3vmn::GetShaderProperties(void)
	{
		AEGLSLProgram3vm::GetShaderProperties();
		this->GetAttribute(a_normal);
	}

	bool AEGLSLProgram3vmn::BindData(
			const AEMatrix4f4 &object,
			const AEMatrix4f4 &camera,
			const AEMatrix4f4 &projection,
			AEMesh * mesh,
			AEMaterial *mat)
	{
		if(!AEGLSLProgram3vm::BindData(object,camera,projection,mesh,mat))
			return false;

		if(a_normal.id!=-1&&mesh->nrmcount)
		{
			glBindBuffer(GL_ARRAY_BUFFER,mesh->idnrm);
			glVertexAttribPointer(a_normal.id,3,GL_FLOAT,GL_FALSE,sizeof(AEVector3f),0);
			glEnableVertexAttribArray(a_normal.id);
		}

		return true;
	}

	void AEGLSLProgram3vmn::UnbindData(void)
	{
		AEGLSLProgram3vm::UnbindData();

		int result;
		glGetVertexAttribiv(a_normal.id,GL_VERTEX_ATTRIB_ARRAY_ENABLED,&result);
		if(result)
			glDisableVertexAttribArray(a_normal.id);
	}

	AEGLSLProgram3vmnl::AEGLSLProgram3vmnl(void)
	{
		u_shaded.name=			"u_shaded";			u_shaded.type=GL_INT;
		u_lights_type.name=		"u_lights.type";	u_lights_type.type=GL_INT;
		u_lights_position.name=	"u_lights.position";u_lights_position.type=GL_FLOAT_VEC3;
		u_lights_rotation.name=	"u_lights.rotation";u_lights_rotation.type=GL_FLOAT_VEC3;
		u_lights_color.name=	"u_lights.color";	u_lights_color.type=GL_FLOAT_VEC4;
		u_lights_attenuation.name=	"u_lights.attenuation"; u_lights_attenuation.type=GL_FLOAT_VEC3;
		u_lights_spot.name=		"u_lights.spot";	u_lights_spot.type=GL_FLOAT_VEC2;
		u_light_num.name=		"u_light_num";		u_light_num.type=GL_INT;
	}

	void AEGLSLProgram3vmnl::GetShaderProperties(void)
	{
		AEGLSLProgram3vmn::GetShaderProperties();
		printf("object_mat -> %d;\ncamera_mat -> %d;\n",u_object_matrix.id,u_camera_matrix.id);
		// this->GetAttribute(a_normal);

		this->GetUniform(u_shaded);
		this->GetUniform(u_lights_type);
		this->GetUniform(u_lights_position);
		this->GetUniform(u_lights_rotation);
		this->GetUniform(u_lights_color);
		this->GetUniform(u_lights_attenuation);
		this->GetUniform(u_lights_spot);
		this->GetUniform(u_light_num);
	}

	bool AEGLSLProgram3vmnl::BindData(
			const AEMatrix4f4 &object,
			const AEMatrix4f4 &camera,
			const AEMatrix4f4 &projection,
			AEMesh * mesh,
			AEMaterial *mat,
			AELightingCache &lcache)
	{
		if(!AEGLSLProgram3vmn::BindData(object,camera,projection,mesh,mat))
			return false;

		if(mat)
		{
			glUniform1i(u_shaded.id,mat->shading);
			if(!mat->shading) return true;
		}
		else
			glUniform1i(u_shaded.id,0);

		int cnt=lcache.arr_length;	//TODO constrain max light number
		glUniform1i(u_light_num.id,cnt);
		if(!cnt) return true;


		glBindBuffer(GL_ARRAY_BUFFER,0);

		glUniform1iv(u_lights_type.id,cnt,&lcache.arr_type[0]);
		glUniform3fv(u_lights_position.id,cnt,(const GLfloat*)&lcache.arr_position[0]);
		glUniform3fv(u_lights_rotation.id,cnt,(const GLfloat*)&lcache.arr_rotation[0]);
		glUniform4fv(u_lights_color.id,cnt,(const GLfloat*)&lcache.arr_color[0]);
		glUniform3fv(u_lights_attenuation.id,cnt,(const GLfloat*)&lcache.arr_attenuation[0]);
		glUniform2fv(u_lights_spot.id,cnt,(const GLfloat*)&lcache.arr_spot[0]);

		return true;
	}

	void AEGLSLProgram3vmnl::UnbindData(void)
	{
		AEGLSLProgram3vmn::UnbindData();
	}

	AEGLSLProgram2vsquare::AEGLSLProgram2vsquare(void)
	{
		a_position.name="a_position";

		u_size.name="u_size";
		u_source0.name="u_source0";
		u_source1.name="u_source1";
		u_source2.name="u_source2";
		u_source3.name="u_source3";
		u_data0.name="u_data0";
		u_data1.name="u_data1";
	}

	void AEGLSLProgram2vsquare::GetShaderProperties(void)
	{
		this->GetAttribute(a_position);

		this->GetUniform(u_size);
		this->GetUniform(u_source0);
		this->GetUniform(u_source1);
		this->GetUniform(u_source2);
		this->GetUniform(u_source3);
		this->GetUniform(u_data0);
		this->GetUniform(u_data1);
	}

	bool AEGLSLProgram2vsquare::BindData(
			AEMesh * mesh,
			uint texture0,
			uint texture1,
			uint texture2,
			uint texture3,
			AEVector2f size,
			AEVector4f data0,
			AEVector4f data1)
	{
		if(!mesh)
			return false;

		glBindBuffer(GL_ARRAY_BUFFER,mesh->idvtx);
		glVertexAttribPointer(a_position.id,3,GL_FLOAT,GL_FALSE,0,0);
		glEnableVertexAttribArray(a_position.id);

		glUniform2f(u_size.id,size.X,size.Y);

		glEnable(GL_TEXTURE_2D);

		if(glIsTexture(texture0))
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D,texture0);
			glUniform1i(u_source0.id,0);
		}

		if(glIsTexture(texture1))
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D,texture1);
			glUniform1i(u_source1.id,1);
		}

		if(glIsTexture(texture2))
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D,texture2);
			glUniform1i(u_source2.id,2);
		}

		if(glIsTexture(texture3))
		{
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D,texture3);
			glUniform1i(u_source3.id,3);
		}

		glActiveTexture(GL_TEXTURE0);

		glUniform4f(u_data0.id,data0.X,data0.Y,data0.Z,data0.W);
		glUniform4f(u_data1.id,data1.X,data1.Y,data1.Z,data1.W);

		return true;
	}

	void AEGLSLProgram2vsquare::UnbindData(void)
	{
		glDisableVertexAttribArray(a_position.id);
		// glDisable(GL_TEXTURE_2D);
	}

	AEGLSLProgram2vsquarelight::AEGLSLProgram2vsquarelight(void)
	{
		// u_shaded.name=			"u_shaded";			u_shaded.type=GL_INT;
		u_lights_type.name=		"u_lights.type";	u_lights_type.type=GL_INT;
		u_lights_position.name=	"u_lights.position";u_lights_position.type=GL_FLOAT_VEC3;
		u_lights_rotation.name=	"u_lights.rotation";u_lights_rotation.type=GL_FLOAT_VEC3;
		u_lights_color.name=	"u_lights.color";	u_lights_color.type=GL_FLOAT_VEC4;
		u_lights_attenuation.name=	"u_lights.attenuation"; u_lights_attenuation.type=GL_FLOAT_VEC3;
		u_lights_spot.name=		"u_lights.spot";	u_lights_spot.type=GL_FLOAT_VEC2;
		u_light_num.name=		"u_light_num";		u_light_num.type=GL_INT;
	}

	void AEGLSLProgram2vsquarelight::GetShaderProperties(void)
	{
		AEGLSLProgram2vsquare::GetShaderProperties();
		// this->GetAttribute(a_normal);

		// this->GetUniform(u_shaded);
		this->GetUniform(u_lights_type);
		this->GetUniform(u_lights_position);
		this->GetUniform(u_lights_rotation);
		this->GetUniform(u_lights_color);
		this->GetUniform(u_lights_attenuation);
		this->GetUniform(u_lights_spot);
		this->GetUniform(u_light_num);
	}

	bool AEGLSLProgram2vsquarelight::BindData(
			AEMesh * mesh,
			uint texture0,
			uint texture1,
			uint texture2,
			uint texture3,
			AEVector2f size,
			AEVector4f data0,
			AEVector4f data1,
			AELightingCache *lcache)
	{
		if(!AEGLSLProgram2vsquare::BindData(mesh,texture0,texture1,texture2,texture3,size,data0,data1))
			return false;

		// if(mat)
		// {
		// 	glUniform1i(u_shaded.id,mat->shading);
		// 	if(!mat->shading) return true;
		// }
		// else
		// 	glUniform1i(u_shaded.id,0);

		if(!lcache) return true;
		int cnt=lcache->arr_length;	//TODO constrain max light number
		glUniform1i(u_light_num.id,cnt);
		if(!cnt) return true;


		glBindBuffer(GL_ARRAY_BUFFER,0);

		glUniform1iv(u_lights_type.id,cnt,&lcache->arr_type[0]);
		glUniform3fv(u_lights_position.id,cnt,(const GLfloat*)&lcache->arr_position[0]);
		glUniform3fv(u_lights_rotation.id,cnt,(const GLfloat*)&lcache->arr_rotation[0]);
		glUniform4fv(u_lights_color.id,cnt,(const GLfloat*)&lcache->arr_color[0]);
		glUniform3fv(u_lights_attenuation.id,cnt,(const GLfloat*)&lcache->arr_attenuation[0]);
		glUniform2fv(u_lights_spot.id,cnt,(const GLfloat*)&lcache->arr_spot[0]);

		return true;
	}

	void AEGLSLProgram2vsquarelight::UnbindData(void)
	{
		AEGLSLProgram2vsquare::UnbindData();
		// glDisableVertexAttribArray(a_normal.id);
	}
}
