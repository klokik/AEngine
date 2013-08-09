#include "AEGLESRender.h"


namespace aengine
{
	int AEGLESRenderUnit::Init(uint16_t _width,uint16_t _height)
	{
		width=_width;
		height=_height;

		return AE_OK;
	}

	void AEGLESRenderUnit::Resize(uint16_t _width,uint16_t _height)
	{
		width=_width;
		height=_height;
	}

	void AEGLESRenderUnit::CacheScene(AEScene *scene)
	{
		// nop
	}

	void AEGLESRenderUnit::CacheClear(void)
	{
		// nop
	}

	void AEGLESRenderUnit::Render(AEObjectCamera *camera)
	{
		// nop
	}

	AEGLESRenderUnit::~AEGLESRenderUnit(void)
	{
		// nop
	}
}