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

namespace aengine
{
	class AETypeCache
	{
	public:
		std::vector<AEObject*> meshes;
		std::vector<AEObject*> empties;
		std::vector<AEObjectJoint*> joints;

		std::vector<AEObjectSprite*> sprites_persp;
		std::vector<AEObjectSprite*> sprites_ortho;

		void Clear(void)
		{
			meshes.clear();
			empties.clear();
			joints.clear();
			sprites_persp.clear();
			sprites_ortho.clear();
		}
	};
}

#endif /* AETYPECACHE_H_ */
