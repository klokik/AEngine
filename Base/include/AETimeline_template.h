/*
 * AETimeline.h
 *
 *  Created on: Aug 19, 2011
 *      Author: klokik
 */

#ifndef AETIMELINE_H_
#define AETIMELINE_H_

#include <map>

template<class T>
class AETimeline
{
protected:
	std::map<float,T> keys;
	T* varptr;
	float pos;
	float length;
	float stepsize;

	bool stop;

	//void SetValue(void);

public:
	AETimeline(T* var,float len=1.0f);

	void SetKey(float offset,T value);

	void SetValue(void);

	AETimeline<T>& operator++();
	AETimeline<T>& operator++(int);

	AETimeline<T>& operator--();
	AETimeline<T>& operator--(int);

	bool EOT(void);


	virtual ~AETimeline(void);
};

template<class T> void AETimeline<T>::SetValue(void)
{
	if(!this->keys.empty())
	{
		std::pair<float,T> L,U;

		//Calculating lower and upper key of current position
		U=*this->keys.upper_bound(this->pos);
		L=*(--this->keys.upper_bound(this->pos));

		float dist=L.first-U.first;

		T diff=L.second-U.second;

		if(dist!=0)
			*this->varptr=L.second+diff*((this->pos-L.first)/dist);
	}
}

template<class T> AETimeline<T>::AETimeline(T* var,float len)
{
	this->length=len;
	this->varptr=var;
	this->pos=0;
	this->stop=true;
	this->stepsize=this->length/100;
}

template<class T> void AETimeline<T>::SetKey(float offset,T value)
{
	std::pair<float,T> p;
	p.first=offset;
	p.second=value;

	this->keys.insert(p);
}

template<class T> inline AETimeline<T>& AETimeline<T>::operator ++()
{
	if(this->pos+this->stepsize<=this->length)
	{
		this->pos+=this->stepsize;
	}
	else
	{
		this->pos=this->length;
	}
	return *this;
}

template<class T> inline AETimeline<T>& AETimeline<T>::operator ++(int)
{
	if(this->pos+this->stepsize<=this->length)
	{
		this->pos+=this->stepsize;
	}
	else
	{
		this->pos=this->length;
	}
	return *this;
}

template<class T> inline AETimeline<T>& AETimeline<T>::operator --()
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

template<class T> inline AETimeline<T>& AETimeline<T>::operator --(int)
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

template<class T> bool AETimeline<T>::EOT(void)
{
	if(this->length==this->pos)
		return true;
	else
		return false;
}

template<class T> AETimeline<T>::~AETimeline(void)
{
	this->keys.clear();
}

#endif /* AETIMELINE_H_ */
