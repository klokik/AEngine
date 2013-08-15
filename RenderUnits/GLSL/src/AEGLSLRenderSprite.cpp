/*
 * AEGLSLRenderSprite.cpp
 *
 *  Created on: Feb 12, 2012
 *      Author: klokik
 */

#include <math.h>
#include <stdio.h>

#include "AEGLHeader.h"
#include "AEGLSLRender.h"

const float rad=57.29577951f;

//void AEGLSLRenderUnit::RenderSpritePersp(const AESpriteObject & obj)
//{
//	//Bind Buffers
//	int textured=false;
//	if(obj.material)
//	{
//		AEColor col=this->scene->materials[obj.material].diffuse;
//		glUniform4f(u_color,col.R,col.G,col.B,col.A);
//
//		if(this->scene->materials[obj.material].texture)
//		{
//			if(this->scene->materials[obj.material].texture->size>0)
//			{
//				glEnable(GL_TEXTURE_2D);
//				glBindTexture(GL_TEXTURE_2D,this->scene->materials[obj.material].texture->id);
//				glUniform1i(u_texture0,0);
//
//				textured=AE_TRUE;
//			}
//		}
//	}
//
//	glUniform1i(u_istextured,textured);
//
//	glBindBuffer(GL_ARRAY_BUFFER,this->sprite_mesh.idvtx);
//	glVertexAttribPointer(a_vertex,3,GL_FLOAT,GL_FALSE,sizeof(AEVertexf),0);
//	glBindBuffer(GL_ARRAY_BUFFER,this->sprite_mesh.iduv);
//	glVertexAttribPointer(a_texcoord,2,GL_FLOAT,GL_FALSE,sizeof(AEVertexf),BUFFER_OFFSET(sizeof(AEVertexf)-sizeof(float)*2));
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->sprite_mesh.idfce);
//
//	glEnableVertexAttribArray(a_vertex);
//	glEnableVertexAttribArray(a_texcoord);
//
//	glPushMatrix();
//
//	glTranslatef(obj.translate.X,obj.translate.Y,obj.translate.Z);
//
//	glRotatef(this->curCam->rotate.Y,0.0,-1.0,0.0);
//	glRotatef(this->curCam->rotate.X,-1.0,0.0,0.0);
//	glRotatef(this->curCam->rotate.Z,0.0,0.0,-1.0);
//
//	glScalef(obj.scale.X,obj.scale.Y,1.0f);
//
//	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
//
//	glPopMatrix();
//
//	glDisableVertexAttribArray(a_vertex);
//	glDisableVertexAttribArray(a_texcoord);
//	glDisable(GL_TEXTURE_2D);
//}
//
//void AEGLSLRenderUnit::RenderSpritesOrtho(void)
//{
//	this->p_basic->Use();
//
//	glBindBuffer(GL_ARRAY_BUFFER,this->sprite_mesh.idvtx);
//	glVertexAttribPointer(a_vertex,3,GL_FLOAT,GL_FALSE,sizeof(AEVertexf),0);
//	glBindBuffer(GL_ARRAY_BUFFER,this->sprite_mesh.iduv);
//	glVertexAttribPointer(a_texcoord,2,GL_FLOAT,GL_FALSE,sizeof(AEVertexf),BUFFER_OFFSET(sizeof(AEVertexf)-sizeof(float)*2));
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->sprite_mesh.idfce);
//
//	glEnableVertexAttribArray(a_vertex);
//	glEnableVertexAttribArray(a_texcoord);
//
//	this->Set2DMode();
//
//	for(unsigned int q=0;q<this->osprite.size();q++)
//	{
//		AESpriteObject* obj=osprite[q];
//
//		int textured=0;
//
//		if(obj->material)
//		{
//			AEColor col=this->scene->materials[obj->material].diffuse;
//			glUniform4f(u_color,col.R,col.G,col.B,col.A);
//
//			if(this->scene->materials[obj->material].texture)
//			{
//				if(this->scene->materials[obj->material].texture->size>0)
//				{
//					glEnable(GL_TEXTURE_2D);
//					glBindTexture(GL_TEXTURE_2D,this->scene->materials[obj->material].texture->id);
//
//					glUniform1i(u_texture0,0);
//
//					textured=AE_TRUE;
//				}
//			}
//		}
//
//		glUniform1i(u_istextured,textured);
//
//		glLoadIdentity();
//
//		glTranslatef(obj->translate.X+obj->rotate.X,obj->translate.Y+obj->rotate.Y,0.0f);
//		glRotatef(obj->rotate.Z,0.0f,0.0f,1.0f);
//		glTranslatef(-obj->rotate.X,-obj->rotate.Y,0.0f);
//
//		glScalef(obj->scale.X,-obj->scale.Y,1.0f);
//
//		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
//
//		glDisable(GL_TEXTURE_2D);
//	}
//
//	glDisableVertexAttribArray(a_vertex);
//	glDisableVertexAttribArray(a_texcoord);
//
//	this->PopMode();
//}

