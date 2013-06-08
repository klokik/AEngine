/*
 * AETimelineManager.h
 *
 *  Created on: Oct 27, 2011
 *      Author: klokik
 */

#ifndef AETIMELINEMANAGER_H_
#define AETIMELINEMANAGER_H_

#include <vector>

#include "AETimeline.h"

class AETimelineManager
{
private:


public:
	std::vector<AETimeline*> timelines;
	bool Autoremove;

	AETimelineManager(void);

	AETimeline *Add(float* var,float len=1.0f,bool repeat=true);
	void Remove(float* var);
	void Clear(void);

	void Step(float interval=-1);

	void Start(float time=0);	//Async start
	void Stop(void);			//Stop async process

	virtual ~AETimelineManager(void);
};

#endif /* AETIMELINEMANAGER_H_ */
