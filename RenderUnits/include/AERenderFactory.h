/*
 * AERenderFactory.h
 *
 *  Created on: Feb 1, 2012
 *      Author: klokik
 */

#ifndef AERENDERFACTORY_H_
#define AERENDERFACTORY_H_

#include <vector>
#include <string>

#include "AERender.h"


namespace aengine
{
	class AERenderFactory
	{
		std::vector<std::string> units;

	public:
		AERenderFactory();

		const std::vector<std::string> &ListAvailableRenderUnits(void);

		AERenderUnit *GetRenderUnit();
		AERenderUnit *GetRenderUnit(uint32_t type);
		AERenderUnit *GetRenderUnit(const char *type);

		virtual ~AERenderFactory();
	};
}

#endif /* AERENDERFACTORY_H_ */
