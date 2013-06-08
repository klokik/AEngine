/*
 * AEProperty.h
 *
 *  Created on: Aug 19, 2011
 *      Author: klokik
 */

#ifndef AEPROPERTY_H_
#define AEPROPERTY_H_

#include "AEDefines.h"

template<class T>
class AEProperty
{
private:

public:
	AEProperty();
	AEProperty(T val);

	T Value;

	unsigned char Type;

	AEProperty &operator =(AEProperty& prop);
	AEProperty &operator =(T prop);

	operator T(void);

	AEProperty &operator +(T& val);
	AEProperty &operator -(T& val);
	AEProperty &operator *(T& val);
	AEProperty &operator /(T& val);

	AEProperty &operator +=(T& val);
	AEProperty &operator -=(T& val);
	AEProperty &operator *=(T& val);
	AEProperty &operator /=(T& val);

	virtual ~AEProperty();
};

template<class T>
AEProperty<T>::AEProperty()
{
	// TODO Auto-generated constructor stub

}

template<class T>
AEProperty<T>::AEProperty(T val)
{
	this->Value=val;
}

template<class T>
AEProperty<T> &AEProperty<T>::operator =(AEProperty<T>& prop)
{
	this->Value=prop.Value;
	this->Type=prop.Type;
	//TODO: copy timeline

	return *this;
}

template<class T>
AEProperty<T> &AEProperty<T>::operator =(T prop)
{
	this->Value=prop;

	return *this;
}

template<class T>
AEProperty<T>::operator T(void)
{
	return this->Value;
}

template<class T>
AEProperty<T>::~AEProperty()
{
	// TODO Auto-generated destructor stub
}

template<class T>
AEProperty<T> &AEProperty<T>::operator +(T& val)
{
	this->Value=this->Value+val;

	return this;
}

template<class T>
AEProperty<T> &AEProperty<T>::operator -(T& val)
{
	this->Value=this->Value-val;

	return this;
}

template<class T>
AEProperty<T> &AEProperty<T>::operator *(T& val)
{
	this->Value=this->Value*val;

	return this;
}

template<class T>
AEProperty<T> &AEProperty<T>::operator /(T& val)
{
	this->Value=this->Value/val;

	return this;
}

template<class T>
AEProperty<T> &AEProperty<T>::operator +=(T& val)
{
	this->Value=this->Value+val;

	return this;
}

template<class T>
AEProperty<T> &AEProperty<T>::operator -=(T& val)
{
	this->Value=this->Value-val;

	return this;
}

template<class T>
AEProperty<T> &AEProperty<T>::operator *=(T& val)
{
	this->Value=this->Value*val;

	return this;
}

template<class T>
AEProperty<T> &AEProperty<T>::operator /=(T& val)
{
	this->Value=this->Value/val;

	return this;
}

#endif /* AEPROPERTY_H_ */
