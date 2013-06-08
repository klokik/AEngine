/*
 * AETimeline.cpp
 *
 *  Created on: Oct 27, 2011
 *      Author: klokik
 */

#include "AETimeline.h"

void AETimeline::SetValue(void)
{
	if(!this->keys.empty())
	{
		std::pair<float,float> L,U;

		//Calculating lower and upper key of current position
		U=*this->keys.upper_bound(this->pos);
		L=*(--this->keys.upper_bound(this->pos));

		float dist=L.first-U.first;

		float diff=L.second-U.second;

		if(dist!=0)
			*this->varptr=L.second+diff*((this->pos-L.first)/dist);
	}
}

AETimeline::AETimeline(float* _var,float _len,bool _repeat)
{
	this->length=_len;
	this->varptr=_var;
	this->pos=0;
	this->stop=true;
	this->stepsize=this->length/100;
	this->repeat=_repeat;
}

void AETimeline::SetKey(float offset,float value)
{
	std::pair<float,float> p;
	p.first=offset;
	p.second=value;

	this->keys.insert(p);
}

inline AETimeline& AETimeline::operator ++()
{
	if(this->pos+this->stepsize<=this->length)
	{
		this->pos+=this->stepsize;
	}
	else
	{
		if(repeat)
			this->pos+=this->stepsize-this->length;
		else
			this->pos=this->length;
	}
	return *this;
}

inline AETimeline& AETimeline::operator ++(int)
{
	this->operator ++();
	return *this;
}

inline AETimeline& AETimeline::operator --()
{
	if(this->pos-this->stepsize<=0.0f)
	{
		this->pos-=this->stepsize;
	}
	else
	{
		this->pos=0;
	}
	return *this;
}

inline AETimeline& AETimeline::operator --(int)
{
	this->operator --();
	return *this;
}

float AETimeline::Step(float interval)
{
	this->stepsize=interval/1000;
	this->operator ++();
	this->SetValue();
	return *this->varptr;
}

bool AETimeline::EOT(void)
{
	if(this->length==this->pos)
		return true;
	else
		return false;
}

AETimeline::~AETimeline(void)
{
	this->keys.clear();
}

