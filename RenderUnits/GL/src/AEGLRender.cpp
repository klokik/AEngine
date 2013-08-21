/*
 * AEGLRenderer.cpp
 *
 *  Created on: Sep 2, 2011
 *      Author: klokik
 */

#include <string.h>
#include <stdlib.h>

#include "AEGLHeader.h"
#include "AEDefines.h"
#include "AEGLRender.h"
#include "AEDebug.h"


namespace aengine
{
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
		// glShadeModel(GL_SMOOTH);			// Enable smooth shading
		//glClearColor(0.0f,0.2f,0.2f,0.0f);	// Color to use for clearing screen
		glClearColor(0.0f,0.0f,0.0f,0.0f);
		glClearDepthf(1.0f);					// Specify the depth buffer cleaning value
		glDepthFunc(GL_LEQUAL);				// The type of depth test to do
#if !defined(AE_NEW_GL_CONTEXT)
		glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST); //type of perspective calculations;
#endif

		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		glBlendColor(0.0f,0.0f,0.0f,1.0f);

		this->InitBasicGeometry();

		return this->CheckError();
	}

	void AEGLRenderUnit::Resize(uint16_t _width,uint16_t _height)
	{
		this->width=_width;
		this->height=_height;

		// protect from division by zero
		if(height==0) height=1;
		if(width==0) width=1;

		glViewport(0,0,(GLfloat)width,(GLfloat)height); // Set up viewport

		// recalculate orthographic matrix
		float o_mtx[16]={
			2.0f/width, 0, 0,0,
			0, 2.0f/height, 0,0,
			0,0,-1,0,
			-1,-1,0,1
		};
		orthomatrix=o_mtx;
	}

	void AEGLRenderUnit::QueueObject(AEObject * obj)
	{
		switch(obj->type)
		{
		case AE_OBJ_EMPTY:
			this->type_cache.empties.push_back(obj);
			break;
		case AE_OBJ_MESH:
			this->type_cache.meshes.push_back(obj);
			// check if mesh exists and is cached
			if((static_cast<AEObjectMesh*>(obj))->mesh&&!(static_cast<AEObjectMesh*>(obj))->mesh->cached)
				CacheObject(obj);
			break;
		case AE_OBJ_SPRITE:
			if(obj->projection==AE_ORTHOGRAPHIC)
				this->type_cache.sprites_ortho.push_back(static_cast<AEObjectSprite*>(obj));
			else
				this->type_cache.sprites_persp.push_back(static_cast<AEObjectSprite*>(obj));
			break;
		case AE_OBJ_TEXT:
			this->type_cache.texts.push_back(static_cast<AEObjectText*>(obj));
			break;
		case AE_OBJ_JOINT:
			this->type_cache.joints.push_back(static_cast<AEObjectJoint*>(obj));
			break;
		// case AE_OBJ_CAMERA:
		// 	break;
		default:
			this->type_cache.empties.push_back(obj);
			//render empty for debug purposes
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

		this->SetFixedProjectionMatrix();

		if(camera!=NULL)
		{
			this->curCam=camera;

			camera->CalculateCameraMatrix(false);
			cammatrix=camera->GetCameraMatrix();

			camera->ratio=width/(float)height;
			camera->InvalidateProjection();
			prjmatrix=camera->GetProjectionMatrix();
		}


		//build light array
		lighting_cache.BuildRelativeArray(camera);

		if(this->scene==NULL)
		{
			AEPrintLog("Specify scene to render, or use CacheScene");
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

		// this->RenderMeshes();
		this->RenderEmpties();
		this->RenderJoints();

		// this->RenderSpritesPersp();
		// this->RenderSpritesOrtho();

		// this->RenderTexts();
	}

	void AEGLRenderUnit::Set2DMode(void)
	{
		prjmatrix.PushMatrix();

		prjmatrix=orthomatrix;

//		glPushAttrib(GL_DEPTH_BUFFER_BIT);
//		glDisable(GL_DEPTH_TEST);
	}

	void AEGLRenderUnit::PopMode(void)
	{
		prjmatrix.PopMatrix();

//		glPopAttrib();
	}

	void AEGLRenderUnit::SetFixedProjectionMatrix()
	{
#if !defined(AE_NEW_GL_CONTEXT)
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMultMatrixf(prjmatrix);
		glMatrixMode(GL_MODELVIEW);
#endif
	}

	int AEGLRenderUnit::CheckError(void)
	{
		GLenum err=glGetError();

		switch(err)
		{
		case GL_NO_ERROR:
			return AE_OK;
		case GL_INVALID_ENUM:
			AEPrintLog("err: invalid enumeration;");
			break;
		case GL_INVALID_VALUE:
			AEPrintLog("err: invalid value;");
			break;
		case GL_INVALID_OPERATION:
			AEPrintLog("err: invalid operation;");
			break;
		case GL_OUT_OF_MEMORY:
			AEPrintLog("err: out of memory;");
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
#if !defined(AE_NEW_GL_CONTEXT)
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
#endif
		return false;
	}

	AEGLRenderUnit::~AEGLRenderUnit(void)
	{
		this->CacheClear();
	}
}
