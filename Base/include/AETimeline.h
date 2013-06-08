/*
 * AETimeline.h
 *
 *  Created on: Aug 19, 2011
 *      Author: klokik
 */

#ifndef AETIMELINE_H_
#define AETIMELINE_H_

#include <map>

class AETimeline
{
protected:
	std::map<float,float> keys;
	float* varptr;
	float pos;
	float length;
	float stepsize;

	bool stop;

	//void SetValue(void);

public:
	bool repeat;

	AETimeline(float* _var,float _len=1.0f,bool _repeat=true);

	void SetKey(float offset,float value);

	void SetValue(void);

	AETimeline& operator++();
	AETimeline& operator++(int);

	AETimeline& operator--();
	AETimeline& operator--(int);

	float Step(float interval=-1);

	bool EOT(void);

	virtual ~AETimeline(void);
};

#endif /* AETIMELINE_H_ */
