/*
 * AECOLLADALoader.h
 *
 *  Created on: Oct 20, 2012
 *      Author: klokik
 */

#ifndef AECOLLADALOADER_H_
#define AECOLLADALOADER_H_

// #define COLLADA_DOM_USING_150

#include <dae.h>

#include "AEScene.h"
#include "AEMesh.h"
#include "AEObjectCamera.h"


namespace aengine
{
	class AECOLLADALoader
	{
	protected:
		//Dae file
		DAE dae;

		//<COLLADA> root node
		daeElement *root;

		daeElement *library_visual_scenes;
		daeElement *library_geometries;
		daeElement *library_materials;
		daeElement *library_effects;

		AEScene *scene;

		int x_up,y_up,z_up;

		void processVisualScenes(void);
		void processSubNodes(daeElement *node_parent,AEObject *parent);
		void processGeometries(void);
		void processMaterials(void);

		AEMesh *processGeometry(daeElement *geometry);
		AEMaterial *processMaterial(daeElement *material);
		AEColor getColor(daeElement *ancestor,const char *property);
		float getFloat(daeElement *ancestor,const char *property);
		void processLight(daeElement *light,AEObjectLight *object);
		void processCamera(daeElement *camera,AEObjectCamera *object);

		int processInput(AEMesh *mesh,daeElement *input,
				size_t *voffset,
				size_t *noffset,
				size_t *toffset);

	public:
		AECOLLADALoader(void);

		int Load(AEScene *_scene,const char *filename);
	};
}

#endif /* AECOLLADALOADER_H_ */
