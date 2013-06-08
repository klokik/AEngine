/*
 * AETextBlock.h
 *
 *  Created on: Dec 23, 2011
 *      Author: klokik
 */

#ifndef AETEXTBLOCK_H_
#define AETEXTBLOCK_H_

#include <vector>
#include <string>

#include "AEUnits.h"

typedef struct _aet_item
{
		std::string text;
		float size;
		float spacing;
		AEColor forecolor;
		AEColor bgcolor;
		bool isBold;
		bool isItalic;
		bool isNarrow;
} _aet_item;

class AETextBlock
{
public:
	AETextBlock();

	std::vector<_aet_item> items;

	virtual void Insert(const char *str,unsigned int pos);

	virtual ~AETextBlock();
};

#endif /* AETEXTBLOCK_H_ */
