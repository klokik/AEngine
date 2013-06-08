/*
 * AEGLRenderBG.cpp
 *
 *  Created on: Sep 24, 2011
 *      Author: klokik
 */

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

#include <GL/gl.h>
 
#include "AEGLRender.h"
#include "AEVectorMath.h"

void AEGLRenderUnit::RenderEmpty(AEObject * obj)
{
	glLoadMatrixf(cammatrix*obj->GetWorldMatrix());

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glDisable(GL_TEXTURE_2D);

	glBindBuffer(GL_ARRAY_BUFFER,empty_mesh.idvtx);
	glVertexPointer(3,GL_FLOAT,0,0);
	glBindBuffer(GL_ARRAY_BUFFER,empty_mesh.idnrm);
	glColorPointer(4,GL_FLOAT,0,0);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	glDrawArrays(GL_LINES,0,6);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void AEGLRenderUnit::RenderEmpties(void)
{
	for(size_t q=0;q<type_cache.empties.size();q++)
	{
		RenderEmpty(type_cache.empties[q]);
	}
}

void AEGLRenderUnit::RenderBoundingSphere(AEObject * obj)
{
	AEVector3f tr=obj->GetAbsPosition();

	float arr[]={
			obj->bounding_sphere.radius, 0.0f, 0.0f, 0.0f,
			0.0f, obj->bounding_sphere.radius, 0.0f, 0.0f,
			0.0f, 0.0f, obj->bounding_sphere.radius, 0.0f,
			tr.X, tr.Y, tr.Z, 1.0f};
	AEMatrix4f4 mtx(arr);

	glLoadMatrixf(cammatrix*mtx);

	glColor4fv((const float*)&obj->bounding_sphere.color);
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER,lcube_mesh.idvtx);
	glVertexPointer(3,GL_FLOAT,0,0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,lcube_mesh.idind);

	glDrawElements(GL_LINES,24,GL_UNSIGNED_INT,0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void AEGLRenderUnit::RenderJoint(AEObjectJoint * obj)
{
	glLoadMatrixf(cammatrix.ToArray());

	glDisable(GL_TEXTURE_2D);

	AEVector3f first=obj->GetAbsPosition();
	AEVector4f second = obj->GetWorldMatrix()*vec4f(obj->tail,1);
	glPointSize(5.0f);
	glBegin(GL_POINTS);
		glColor4fv((const float*)&obj->display_color);
		glVertex3f(first.X,first.Y,first.Z);
		glVertex3f(second.X,second.Y,second.Z);
	glEnd();

	glBegin(GL_LINES);
		glVertex3f(first.X,first.Y,first.Z);
		glVertex3f(second.X,second.Y,second.Z);
	glEnd();

//	if(obj->parent == NULL)
//		return;

	// AEVector3f pp=obj->parent->GetAbsPosition();
	// glBegin(GL_LINES);
	// 	glColor4fv((const float*)&obj->display_color);
	// 	glVertex3f(op.X,op.Y,op.Z);
	// 	if(obj->parent!=NULL&&typeid(*obj->parent)==typeid(AEObjectJoint))
	// 		glColor4fv((const float*)&dynamic_cast<AEObjectJoint*>(obj->parent)->display_color);
	// 	glVertex3f(pp.X,pp.Y,pp.Z);
	// glEnd();
	// glEnableClientState(GL_VERTEX_ARRAY);

	// glBindBuffer(GL_ARRAY_BUFFER,tetra_mesh.idvtx);
	// glVertexPointer(3,GL_FLOAT,0,0);
	// glBindBuffer(GL_ARRAY_BUFFER,0);

	// glDrawArrays(GL_LINES,0,12);

	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	// glDisableClientState(GL_VERTEX_ARRAY);
}

void AEGLRenderUnit::RenderJoints(void)
{
	for(size_t q=0;q<type_cache.joints.size();q++)
	{
		RenderJoint(type_cache.joints[q]);
	}
}