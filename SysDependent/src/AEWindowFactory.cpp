#include "AEWindow.h"

#if defined(PLATFORM_xNIX)||defined(PLATFORM_WINDOWS)
	#include "SDL/AESDLWindow.h"
#elif defined(PLATFORM_ANDROID)
	#include "AEAndroidWindow.h"
#elif defined(PLATFORM_BLACKBERRY10)
	#include "AECascadesWindow.h"
#else
	#error No target platform defined
#endif

namespace aengine
{
	AEWindow *GetPlatformWindow(void *engine)
	{
		AEWindow *wnd = nullptr;

		#if defined(PLATFORM_xNIX)||defined(PLATFORM_WINDOWS)
			wnd = new AESDLWindow(engine);
		#elif defined(PLATFORM_ANDROID)
			wnd = new AEAndroidWindow(engine);
		#elif defined(PLATFORM_BLACKBERRY10)
			wnd = new AECascadesWindow(engine);
		#endif

		return wnd;
	}
}