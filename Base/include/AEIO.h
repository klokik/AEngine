/*
 * AEIO.h
 *
 *  Created on: Jan 31, 2012
 *      Author: klokik
 */

#ifndef AEIO_H_
#define AEIO_H_

#include <stdio.h>
#include <iostream>
#include <fstream>

static char *text=NULL;	//move it to cpp file

std::string AEReadTextFileS(const char *name)
{
	std::ifstream fs(name);
	std::string str,tmp;

	if(!fs.is_open())
	{
		return NULL;
	}

	std::getline(fs,str);	//read first line

	while(!fs.eof())
	{
		std::getline(fs,tmp);
		str.append("\n"+tmp);
	}

	fs.close();
	return str;
}

const char *AEReadTextFileC(const char *name)	//char string
{
	if(text!=NULL) {free(text); text=NULL;}

	FILE *f;
	f=fopen(name,"r");
	if(!f)
	{
		printf("err: file %s not found;\n",name);
		return AE_ERR;
	}
	fseek(f,0L,SEEK_END);
	size_t len=ftell(f);
	fseek(f,0L,SEEK_SET);

	text=(char*)malloc(sizeof(char)*(len+1));

	for(size_t q=0;q<len;q++)
		text[q]=fgetc(f);

	text[len]='\0';
	fclose(f);

	return text;
}

const char **AEReadTextFileCml(const char *name) //char string(multiline)
{
	AEReadTextFileC(name);
	return (const char**)&text;
}

#endif /* AEIO_H_ */
