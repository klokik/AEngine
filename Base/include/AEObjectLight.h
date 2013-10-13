/*
 * AEObjectLight.h
 *
 *  Created on: Mar 13, 2012
 *      Author: klokik
 */

#ifndef AEOBJECTLIGHT_H_
#define AEOBJECTLIGHT_H_

#include "AEUnits.h"
#include "AEObject.h"


namespace aengine
{
	class AEObjectLight: public AEObject
	{
	private:
		void Init(void);

	public:
		AEObjectLight(void);
		AEObjectLight(AE_LIGHT_TYPE ltype);

		AE_LIGHT_TYPE light_type;
		AEColor color;		//Alpha component is used as intensity
		AEVector3f attenuation; //constant,linear,square
		AEVector2f spot;

		virtual ~AEObjectLight(void);
	};
}

#endif /* AEOBJECTLIGHT_H_ */
