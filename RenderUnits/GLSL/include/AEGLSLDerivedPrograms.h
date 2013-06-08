/*
 * AEGLSLDerivedPrograms.h
 *
 *  Created on: Mar 14, 2012
 *      Author: klokik
 */

#ifndef AEGLSLDERIVEDPROGRAMS_H_
#define AEGLSLDERIVEDPROGRAMS_H_

#include "AEMesh.h"
#include "AEMaterial.h"
#include "AEGLSLProgramManager.h"
#include "AELightingCache.h"

class AEGLSLProgram3vc: public AEGLSLProgram
{
public:

	AEGLSLProperty u_object_matrix;
	AEGLSLProperty u_camera_matrix;
	AEGLSLProperty u_projection_matrix;
	AEGLSLProperty a_vertex;
	AEGLSLProperty a_color,u_color;

	AEGLSLProgram3vc(void);

	virtual void GetShaderProperties(void);

	virtual bool BindData(
			const AEMatrix4f4 &object,
			const AEMatrix4f4 &camera,
			const AEMatrix4f4 &projection,
			int vbo,int cbo,
			AEColor &color);
	virtual void UnbindData(void);
};

class AEGLSLProgram3vm: public AEGLSLProgram3vc
{
public:
	AEGLSLProperty u_istextured0,u_texture0,a_texcoord0;

	bool texturing;

	AEGLSLProgram3vm(void);

	virtual void GetShaderProperties(void);

	virtual bool BindData(
			const AEMatrix4f4 &object,
			const AEMatrix4f4 &camera,
			const AEMatrix4f4 &projection,
			AEMesh * mesh,
			AEMaterial *mat);
	virtual void UnbindData(void);
};

class AEGLSLProgram3vmn: public AEGLSLProgram3vm
{
public:
	AEGLSLProperty a_normal;

	AEGLSLProgram3vmn(void);

	virtual void GetShaderProperties(void);

	virtual bool BindData(
		const AEMatrix4f4 &object,
		const AEMatrix4f4 &camera,
		const AEMatrix4f4 &projection,
		AEMesh * mesh,
		AEMaterial *mat);
	virtual void UnbindData(void);
};

class AEGLSLProgram3vmnl: public AEGLSLProgram3vmn
{
public:
	AEGLSLProperty u_shaded;
	AEGLSLProperty u_lights_type;
	AEGLSLProperty u_lights_position;
	AEGLSLProperty u_lights_rotation;
	AEGLSLProperty u_lights_color;
	AEGLSLProperty u_lights_attenuation;
	AEGLSLProperty u_lights_spot;
	AEGLSLProperty u_light_num;

	AEGLSLProgram3vmnl(void);

	virtual void GetShaderProperties(void);

	virtual bool BindData(
			const AEMatrix4f4 &object,
			const AEMatrix4f4 &camera,
			const AEMatrix4f4 &projection,
			AEMesh * mesh,
			AEMaterial *mat,
			AELightingCache *lcache);
	virtual void UnbindData(void);
};

class AEGLSLProgram2vsquare: public AEGLSLProgram
{
public:
	AEGLSLProperty a_position;

	AEGLSLProperty u_source0;
	AEGLSLProperty u_source1;
	AEGLSLProperty u_source2;
	AEGLSLProperty u_source3;
	
	AEGLSLProperty u_size;

	AEGLSLProperty u_data0;
	AEGLSLProperty u_data1;

	AEGLSLProgram2vsquare(void);

	virtual void GetShaderProperties(void);

	virtual bool BindData(
		AEMesh * mesh,
		uint texture0,
		uint texture1,
		uint texture2,
		uint texture3,
		AEVector2f size,
		AEVector4f data0,
		AEVector4f data1);

	virtual void UnbindData(void);
};

class AEGLSLProgram2vsquarelight: public AEGLSLProgram2vsquare
{
public:
	AEGLSLProperty u_lights_type;
	AEGLSLProperty u_lights_position;
	AEGLSLProperty u_lights_rotation;
	AEGLSLProperty u_lights_color;
	AEGLSLProperty u_lights_attenuation;
	AEGLSLProperty u_lights_spot;
	AEGLSLProperty u_light_num;

	AEGLSLProgram2vsquarelight(void);

	virtual void GetShaderProperties(void);

	virtual bool BindData(
		AEMesh * mesh,
		uint texture0,
		uint texture1,
		uint texture2,
		uint texture3,
		AEVector2f size,
		AEVector4f data0,
		AEVector4f data1,
		AELightingCache *lcache);
	virtual void UnbindData(void);
};

#endif /* AEGLSLDEIVEDPROGRAMS_H_ */
