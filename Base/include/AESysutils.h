/*
 * AESysutils.h
 *
 *  Created on: Dec 26, 2011
 *      Author: klokik
 */

#ifndef AESYSUTILS_H_
#define AESYSUTILS_H_

inline void memset32(void *dest,uint32_t value,size_t size)
{
	//uint32_t *sptr=(uint32_t*)dest;
	//uint32_t *mptr=(uint32_t*)(dest+size);
	//while(sptr++<mptr)
	//	*sptr=value;
	uint32_t *ptr=(uint32_t*)dest;
	for(unsigned int q=0;q<size/sizeof(uint32_t);q++)
		ptr[q]=value;
}

//void memset64( void * dest, uint64_t value, uintptr_t size )
//{
//	uintptr_t i;
//	for( i = 0; i < (size & (~7)); i+=8 )
//	{
//	memcpy( ((char*)dest) + i, &value, 8 );
//	}
//	for( ; i < size; i++ )
//	{
//	((char*)dest)[i] = ((char*)&value)[i&7];
//	}
//}

#endif /* AESYSUTILS_H_ */
