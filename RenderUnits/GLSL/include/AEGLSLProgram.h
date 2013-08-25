/*
 * AEGLSLProgram.h
 *
 *  Created on: Mar 17, 2012
 *      Author: klokik
 */

#ifndef AEGLSLPROGRAM_H_
#define AEGLSLPROGRAM_H_

#include <string>


namespace aengine
{
	class AEGLSLProperty
	{
	public:
		int id;
		std::string name;
		uint32_t type;

		AEGLSLProperty(void);
	};

	class AEGLSLShader
	{
	public:
		uint32_t id;	//READ ONLY !!!
		uint32_t type;	//READ ONLY !!!

		AEGLSLShader(uint32_t _type);

		void ShaderData(std::string data);
		int Compile(bool quiet=false);
		int GetCompileStatus(void);
		std::string GetLog(void);

		virtual ~AEGLSLShader(void);
	};

	class AEGLSLProgram
	{
	//protected:
	//	std::vector<AEGLSLShader*> shaders;

	public:
		uint32_t id;

		AEGLSLProgram(void);

		void Use(void);

		void Attach(AEGLSLShader &shd);
		void Detach(AEGLSLShader &shd);

		int Link(bool quiet=false);
		int GetLinkStatus(void);
		std::string GetLog(void);

		int GetUniform(const char *name);
		int GetAttribute(const char *name);

		void GetUniform(AEGLSLProperty & prop);
		void GetAttribute(AEGLSLProperty & prop);

		//Dummy func! should be overloaded in derived classes
		virtual void GetShaderProperties(void);

		virtual ~AEGLSLProgram(void);
	};
}

#endif /* AEGLSLPROGRAM_H_ */
