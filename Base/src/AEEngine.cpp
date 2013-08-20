/*
 * AEEngine.cpp
 *
 *  Created on: Nov 8, 2011
 *      Author: klokik
 */

#include <cstring>

#include "AEEngine.h"
#include "AEDefines.h"
#include "AERenderFactory.h"
#include "AEWindow.h"
#include "AEDebug.h"

namespace aengine
{
	AEEngine::AEEngine(void)
	{
		this->curCamera=nullptr;
		this->render=nullptr;
		this->window=nullptr;
		this->scene=nullptr;

		this->OnClose	=	nullptr;
		this->OnKeyDown	=	nullptr;
		this->OnKeyUp	=	nullptr;
		this->OnMouseDown=	nullptr;
		this->OnMouseMove=	nullptr;
		this->OnMouseUp	=	nullptr;
		this->OnResize	=	nullptr;
		this->OnStart	=	nullptr;
		this->Refresh	=	nullptr;

		memset(this->keys,0,sizeof(this->keys));
	}

	int AEEngine::Init(uint32_t flags)
	{
		if(flags&AE_INIT_WINDOW)
		{
			this->window=GetPlatformWindow(this);

			if(flags&AE_INIT_RENDER_GL)
			{
				this->window->InitWindow(640,480,24,AE_SDLGL_WINDOW);

				AERenderFactory rfactory;

				this->render=rfactory.GetRenderUnit("GLES");
				if(!this->render)
				{
					AEPrintLog("Incorrect RenderUnit");
					return AE_ERR;
				}
				if(!this->render->Init(640,480))
				{
					delete this->render;
					delete this->window;
					return AE_ERR;
				}
			}
			else
			{
				this->window->InitWindow(640,480,16,NULL);
			}
		}
		if(flags&AE_INIT_SCENE)
		{
			AEPrintLog("Scene created");
			this->scene=new AEScene();

			if(flags&AE_INIT_CAMERA)
			{
				this->curCamera=new AEObjectCamera;
				this->scene->AddObject(this->curCamera);
			}
		}

		return AE_OK;
	}

	int AEEngine::Run(void)
	{
		if(this->window)
			this->window->Run(NULL,false);

		return AE_OK;
	}

	int AEEngine::Stop(void)
	{
		if(this->window)
			this->window->Stop();
		return AE_OK;
	}

	void AEEngine::i_OnClose(int *param)
	{
		if(this->OnClose!=NULL)
			OnClose(param);
	}

	void AEEngine::i_OnKeyDown(int *param)
	{
		this->keys[param[0]]=AE_TRUE;

		if(this->OnKeyDown!=NULL)
			OnKeyDown(param);
	}

	void AEEngine::i_OnKeyUp(int *param)
	{
		this->keys[param[0]]=AE_FALSE;

		if(this->OnKeyUp!=NULL)
			OnKeyUp(param);
	}

	void AEEngine::i_OnMouseDown(int *param)
	{
		if(this->OnMouseDown!=NULL)
			OnMouseDown(param);
	}

	void AEEngine::i_OnMouseMove(int *param)
	{
		if(this->OnMouseMove!=NULL)
			OnMouseMove(param);
	}

	void AEEngine::i_OnMouseUp(int *param)
	{
		if(this->OnMouseUp!=NULL)
			OnMouseUp(param);
	}

	void AEEngine::i_OnResize(int *param)
	{
		int width=param[0];
		int height=param[1];

		this->render->Resize(width,height);

		char buf[256];
		sprintf(buf,"Window resized [%d,%d];\n",width,height);
		AEPrintLog(buf);

		if(this->OnResize!=NULL)
			OnResize(param);
	}

	void AEEngine::i_OnStart(int *param)
	{
		if(this->OnStart!=NULL)
			OnStart(param);
	}

	void AEEngine::i_Refresh(int *param)
	{
		this->scene->animations.Step(param[0]);

		if(this->Refresh!=NULL)
			Refresh(param);

		//TODO: Update scene
		
		this->render->Render(this->curCamera);
		this->window->SwapBuffers();
	}

	unsigned char AEEngine::GetKeyState(short int key) const
	{
		return this->keys[key];
	}

	AEEngine::~AEEngine(void)
	{
		if(this->render)
			delete this->render;
		if(this->window)
			delete this->window;
	}
}
