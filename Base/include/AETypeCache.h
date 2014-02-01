/*
 * AETypeCache.h
 *
 *  Created on: Jun 25, 2012
 *      Author: klokik
 */

#ifndef AETYPECACHE_H_
#define AETYPECACHE_H_

#include <vector>

#include "AEObject.h"
#include "AEObjectSprite.h"
#include "AEObjectJoint.h"
#include "AEObjectText.h"
#include "AEObjectParticleSystem.h"


namespace aengine
{
	class AETypeCache
	{
	public:
		std::vector<AEObject*> meshes;
		std::vector<AEObject*> empties;
		std::vector<AEObjectJoint*> joints;
		std::vector<AEObjectParticleSystem*> particle_systems;


		std::vector<AEObjectSprite*> sprites_persp;
		std::vector<AEObjectSprite*> sprites_ortho;

		std::vector<AEObjectText*> texts;

		void Clear(void)
		{
			meshes.clear();
			empties.clear();
			joints.clear();
			particle_systems.clear();
			sprites_persp.clear();
			sprites_ortho.clear();
			texts.clear();
		}
	};
}

#endif /* AETYPECACHE_H_ */
