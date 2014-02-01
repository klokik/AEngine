/*
 * AEObjectEmpty.h
 *
 *  Created on: Sep 24, 2011
 *      Author: klokik
 */

#ifndef AEOBJECTEMPTY_H_
#define AEOBJECTEMPTY_H_

#include "AEObject.h"


namespace aengine
{
	class AEObjectEmpty: public AEObject
	{
	public:
		AEObjectEmpty();
	};

	class AEObjectDynamicEmpty: public virtual AEObject
	{
	public:
		AEObjectDynamicEmpty();
	};
}

#endif /* AEOBJECTEMPTY_H_ */
