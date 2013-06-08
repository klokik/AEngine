/*
 * AETimelineManager.cpp
 *
 *  Created on: Oct 27, 2011
 *      Author: klokik
 */

#include "AETimelineManager.h"

AETimelineManager::AETimelineManager(void)
{
}

AETimeline *AETimelineManager::Add(float *var, float len, bool repeat)
{
	AETimeline* timeline=new AETimeline(var,len,repeat);
	this->timelines.push_back(timeline);
	return timeline;
}

void AETimelineManager::Remove(float *var)
{
//	for(unsigned int q=0;q<this->timelines.size();q++)
//		if(this->timelines[q])
}

void AETimelineManager::Clear(void)
{
	for(unsigned int q=0;q<this->timelines.size();q++)
		delete this->timelines[q];
	this->timelines.clear();
}

void AETimelineManager::Step(float interval)
{
	for(unsigned int q=0;q<this->timelines.size();q++)
		this->timelines[q]->Step(interval);
}

void AETimelineManager::Start(float time)
{
	//TODO:start new thread with timer, that changes values
}

void AETimelineManager::Stop(void)
{
	//TODO:kill thread
}

AETimelineManager::~AETimelineManager(void)
{
	this->Clear();
}

