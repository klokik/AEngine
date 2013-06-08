/*
 * AETextBlock.cpp
 *
 *  Created on: Dec 23, 2011
 *      Author: klokik
 */

#include "AETextBlock.h"

AETextBlock::AETextBlock()
{
}

void AETextBlock::Insert(const char *str, unsigned int pos)
{
	_aet_item item;
	item.text=str;
	item.bgcolor={0,0,0,0};
	item.forecolor={255,255,255,255};
	item.size=16.0f;
	item.spacing=0.0f;
	item.isBold=item.isItalic=item.isNarrow=false;

	this->items.push_back(item);
}

AETextBlock::~AETextBlock()
{
	this->items.clear();
}

