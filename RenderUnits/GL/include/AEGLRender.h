/*
 * AEGLRender.h
 *
 *  Created on: Sep 2, 2011
 *      Author: klokik
 */

#ifndef AEGLRENDER_H_
#define AEGLRENDER_H_

#include <vector>
#include <typeinfo>

#include "AERender.h"
#include "AEMatrixStack.h"
#include "AEMesh.h"
#include "AELightingCache.h"
#include "AEObjectMesh.h"
#include "AEObjectText.h"
#include "AEObjectJoint.h"
#include "AETypeCache.h"
#include "AEFontBitmap.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace aengine
{
	class AEGLRenderUnit: public AERenderUnit
	{
	protected:
		bool cached;
		//Independent means that this method was called by this class on his own
		// not from, other inhrited class
		bool independent;

		AEScene *scene;

		AEMatrix4f4   identity;

		AEMatrixStack mvsmatrix;
		AEMatrix4f4	  cammatrix;
		AEMatrix4f4   prjmatrix;

		AEObject *curCam;

		std::vector<unsigned int> buffers;
		std::vector<unsigned int> textures;

		AELightingCache lighting_cache;
		AETypeCache	type_cache;

		AEMesh sprite_mesh;
		AEMesh empty_mesh;
		AEMesh lcube_mesh;
		AEMesh tetra_mesh;

		virtual int InitGL(void);

		virtual int InitBasicGeometry(void);
		virtual void FreeBGBuffers(void);
		virtual int InitBGEmpty(void);
		virtual int InitBGSprite(void);
		virtual int InitBGLCube(void);
		virtual int InitBGTetra(void);

		//*--region cache
		virtual int CacheObject(AEObject * obj);
		virtual int CacheTexture(AETexture * tex);
		virtual int CacheMaterials(void);
		virtual int CacheFonts(void);

		virtual int CacheMesh(AEMesh * mesh);
		virtual void GenerateMeshBuffers(AEMesh * mesh);
		virtual int UpdateMeshBuffers(AEMesh * mesh);
		//*/// endregion cache

		virtual void RenderEmpty(AEObject * obj);
		virtual void RenderMesh(AEObjectMesh * obj);
		virtual void RenderJoint(AEObjectJoint * obj);

		virtual void RenderText(AEObjectText *obj);

		virtual void RenderSpritesPersp(void);
		virtual void RenderSpritesOrtho(void);

		virtual void RenderBoundingSphere(AEObject * obj);

		virtual void RenderMeshes(void);
		virtual void RenderEmpties(void);
		virtual void RenderJoints(void);
		virtual void RenderTexts(void);

		///returns "true" if object is textured, and "false" in the other case
		virtual bool ApplyMaterial(AEMaterial *mat);

		virtual void Set2DMode(void);
		virtual void PopMode(void);
		virtual int CheckError(void) const;

		virtual unsigned int GetGLVersion(void);

	public:

		AEGLRenderUnit(void);

		virtual int Init(uint16_t _width,uint16_t _height);
		virtual int Init(uint16_t _width,uint16_t _height,bool independent);
		virtual void Resize(uint16_t _width,uint16_t _height);

		virtual void CacheScene(AEScene *scene);
		virtual void CacheClear(void);

		virtual void QueueObject(AEObject *obj);
		virtual void Render(AEObjectCamera *camera);

		virtual ~AEGLRenderUnit(void);
	};
}

#endif /* AEGLRENDER_H_ */
