/*
 * AEGLRenderer.cpp
 *
 *  Created on: Sep 2, 2011
 *      Author: klokik
 */

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "AEDefines.h"
#include "AEGLRender.h"

AEGLRenderUnit::AEGLRenderUnit(void)
{
	this->cached=false;
	this->curCam=NULL;
	this->scene=NULL;
	this->independent=true;
}

int AEGLRenderUnit::Init(uint16_t _width,uint16_t _height)
{
	return Init(_width,_height,true);
}

int AEGLRenderUnit::Init(uint16_t _width,uint16_t _height,bool independent)
{
	int result=AE_OK;

	result&=this->InitGL();
	result&=this->InitBasicGeometry();

	//Independent means that this method was called by this class on his own
	// not from, other inhrited class
	this->independent=independent;
	if(independent)
	{
		this->Resize(_width,_height);
	}
	else
	{
		this->width=_width;
		this->height=_height;
	}

	return result;
}

int AEGLRenderUnit::InitGL(void)
{
	glShadeModel(GL_SMOOTH);			// Enable smooth shading
	//glClearColor(0.0f,0.2f,0.2f,0.0f);	// Color to use for clearing screen
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClearDepth(1.0f);					// Specify the depth buffer cleaning value
	glDepthFunc(GL_LEQUAL);				// The type of depth test to do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST); //type of perspective calculations;

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glBlendColor(0.0f,0.0f,0.0f,1.0f);

	this->InitBasicGeometry();

	return this->CheckError();
}

void AEGLRenderUnit::Resize(uint16_t _width,uint16_t _height)
{
	this->width=_width;
	this->height=_height;

	GLfloat ratio;									// window aspect ratio width/height
	if(height==0) height=1;	 						//protect from division by zero
	ratio=(GLfloat)width/(GLfloat)height;

	glViewport(0,0,(GLfloat)width,(GLfloat)height); // Set up viewport
	glMatrixMode(GL_PROJECTION);					// Change to the projection
	glLoadIdentity();								// matrix and set viewing volume
	gluPerspective(45.0f,ratio,0.1f,200.0f);		// Set perspective
	glMatrixMode(GL_MODELVIEW);						//Make sure we are changing modelview, not projection
	glLoadIdentity();								// Reset the view
}

void AEGLRenderUnit::QueueObject(AEObject * obj)
{
	switch(obj->type)
	{
	case AE_OBJ_EMPTY:
		//this->RenderEmpty(obj);
		this->type_cache.empties.push_back(obj);
		break;
	case AE_OBJ_MESH:
		//this->RenderMesh(dynamic_cast<AEObjectMesh*>(obj));
		this->type_cache.meshes.push_back(obj);
		// check if mesh is cached
		if((dynamic_cast<AEObjectMesh*>(obj))->mesh&&!(dynamic_cast<AEObjectMesh*>(obj))->mesh->cached)
			CacheObject(obj);
		break;
	case AE_OBJ_SPRITE:
		if(obj->projection==AE_ORTHOGRAPHIC)
			this->type_cache.sprites_ortho.push_back(dynamic_cast<AEObjectSprite*>(obj));
		else
			this->type_cache.sprites_persp.push_back(dynamic_cast<AEObjectSprite*>(obj));
		break;
	case AE_OBJ_TEXT:
		//this->RenderText(dynamic_cast<AEObjectText*>(obj));
		this->type_cache.texts.push_back(dynamic_cast<AEObjectText*>(obj));
		break;
	case AE_OBJ_JOINT:
		//this->RenderJoint(dynamic_cast<AEObjectJoint*>(obj));
		this->type_cache.joints.push_back(dynamic_cast<AEObjectJoint*>(obj));
		break;
	// case AE_OBJ_CAMERA:
	// 	break;
	default:
		this->type_cache.empties.push_back(obj);
		//render empty for debug purposes
		// or better render nothing
		//this->RenderEmpty(obj);
		break;
	}

	//this->RenderBoundingSphere(obj);

	for(size_t q=0;q<obj->children.size();q++)
	{
		this->QueueObject(obj->children[q]);
	}
}

void AEGLRenderUnit::Render(AEObjectCamera *camera)
{
	type_cache.Clear();

	mvsmatrix.SetIdentity();

	if(camera!=NULL)
	{
		this->curCam=camera;

		camera->CalculateCameraMatrix(false);
		cammatrix=camera->GetCameraMatrix();
	}


	//build light array
	lighting_cache.BuildRelativeArray(camera);

	if(this->scene==NULL)
	{
		printf("Please cache scene before usage\n");
		throw 0;
	}

	size_t max=this->scene->objects.size();

	for(size_t q=0;q<max;q++)
	{
		this->scene->objects[q]->CalculateBoundingSphere();
		this->QueueObject(this->scene->objects[q]);
	}

	for(auto joint=type_cache.joints.begin();joint!=type_cache.joints.end();++joint)
		(*joint)->ApplyDeformation();

	if(independent)
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
	}

	this->RenderMeshes();
	this->RenderEmpties();
	this->RenderJoints();

	this->RenderSpritesPersp();
	this->RenderSpritesOrtho();

	this->RenderTexts();
}

void AEGLRenderUnit::Set2DMode(void)
{
	//TODO Use own projection matrices
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,width,0,height,-1.0,1.0);
	float mat[16];
	glGetFloatv(GL_PROJECTION_MATRIX,mat);
	prjmatrix=mat;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glPushAttrib(GL_DEPTH_BUFFER_BIT|GL_LIGHTING_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
}

void AEGLRenderUnit::PopMode(void)
{
	glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	float mat[16];
	glGetFloatv(GL_PROJECTION_MATRIX,mat);
	prjmatrix=mat;
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

int AEGLRenderUnit::CheckError(void) const
{
	GLenum err=glGetError();

	switch(err)
	{
	case GL_NO_ERROR:
		return AE_OK;
	case GL_INVALID_ENUM:
		puts("err: invalid enumeration;");
		break;
	case GL_INVALID_VALUE:
		puts("err: invalid value;");
		break;
	case GL_INVALID_OPERATION:
		puts("err: invalid operation;");
		break;
	case GL_STACK_OVERFLOW:
		puts("err: stack underflow;");
		break;
	case GL_OUT_OF_MEMORY:
		puts("err: out of memory;");
		break;
	case GL_TABLE_TOO_LARGE:
		puts("err: table too large;");
		break;
	}
	return AE_ERR;
}

unsigned int AEGLRenderUnit::GetGLVersion(void)
{
	char ver[3];
	char *sver=(char*)glGetString(GL_VERSION);
	ver[0]=sver[0];
	ver[1]=sver[2];
	ver[2]='\0';
	return atoi(ver);
}

bool AEGLRenderUnit::ApplyMaterial(AEMaterial *mat)
{
	if(!mat)
	{
		glColor3f(1.0f,1.0f,1.0f);
		return false;
	}

	glColor4fv((const float*)&mat->diffuse);

	if(mat->texture)
	{
		if(mat->texture->size>0)
		{
			// glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,mat->texture->id);
			return true;
		}
	}
	return false;
}

AEGLRenderUnit::~AEGLRenderUnit(void)
{
	this->CacheClear();
}

