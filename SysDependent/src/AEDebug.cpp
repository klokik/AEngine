#include "AEDebug.h"

//TODO: move to SysDep

#if defined(PLATFORM_ANDROID)

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "AEngine", __VA_ARGS__))
namespace aengine
{
	void AEPrintLog(std::string str)
	{
		LOGI("%s",str.c_str());
	}
}

#else

#include <iostream>
namespace aengine
{
	void AEPrintLog(std::string str)
	{
		std::cout<<str<<std::endl;
	}
}

#endif