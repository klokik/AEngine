/*
 * main.cpp
 *
 *  Created on: Feb 08, 2013
 *      Author: klokik
 */

#include <stdio.h>
#include <math.h>
#include <GL/gl.h>

#include <iostream>

#include "AEEngine.h"

#include "AETexture.h"
#include "AEVectorMath.h"
#include "Utils/AEUBitMapTextGenerator.h"
#include "AEObjectMesh.h"
#include "AEObjectSprite.h"
#include "AEObjectEmpty.h"
#include "AEObjectText.h"

#include "AEVision.h"


using namespace aengine;

void LoadObjFile(AEMesh *&mesh, const char *path);


AEEngine engine;
AEObjectCamera *camera1,*camera2;
AEObjectMesh *pobj,*plane;
AEObjectLight *plight1;

AEObjectTracker tracker(0);

void Refresh(int *param)
{
	float lambda=0.02f*param[0];
	float mlambda=0.1f*param[0];

	if(engine.GetKeyState(SDLK_SPACE))
		engine.curCamera->Move(vec3f(0,0,-lambda));

	if(engine.GetKeyState(SDLK_DOWN))	engine.curCamera->RelRotate(vec3f(-mlambda,0,0));
	if(engine.GetKeyState(SDLK_UP))		engine.curCamera->RelRotate(vec3f( mlambda,0,0));
	if(engine.GetKeyState(SDLK_RIGHT))  engine.curCamera->RelRotate(vec3f(0,-mlambda,0));
	if(engine.GetKeyState(SDLK_LEFT))   engine.curCamera->RelRotate(vec3f(0, mlambda,0));

	//pobj->SetTranslate(track_position*0.05f+vec3f(0,0,-20));
	if(tracker.IsReady("Ball"))
	{
		AEVector3f pos=vec3f(tracker["Ball"].Y,-tracker["Ball"].X,0);
		engine.curCamera->RelRotate(pos*1.0f);
	}

//	if(track_length!=0)
//	{
//		float tr_delta=track_length-350;
//		engine.curCamera->Move(vec3f(0,0,-tr_delta*0.002f));
//	}

	static int frame=0;
	static int time=0;
	frame++;
	time+=param[0];
	if(time>=5000)
	{
		printf("FPS: %f;\n",frame*1000.0f/time);
		frame=0;
		time=0;
	}
}

void StartInit(int *)
{
	camera1=engine.curCamera;
	camera2=new AEObjectCamera;

	plight1=new AEObjectLight(AE_AMBIENT_LIGHT);
	plight1->color.A=0.5f;

	pobj=new AEObjectMesh;
	pobj->SetTranslate(vec3f(0,0,-10));
	plane=new AEObjectMesh;

	LoadObjFile(pobj->mesh,"../resource/cube_flat.obj");
	LoadObjFile(plane->mesh,"../resource/landskape_texmtn.obj");

	AEMaterial *mat=engine.scene->materials.New();
	engine.scene->materials.LoadTexture(mat->texture,"../resource/grass.jpg");
	//pobj->material=mat;
	plane->material=mat;

	engine.scene->AddObject(pobj);
	engine.scene->AddObject(camera2);
	engine.scene->AddObject(plight1);
	engine.scene->AddObject(plane);

	// AECOLLADALoader loader;
	// loader.Load(engine.scene,"resource/town.dae");

	engine.render->CacheScene(engine.scene);

	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	//SDL_EnableKeyRepeat(10,20);
	
	tracker.RequestObject("Ball");
	tracker.Run(true);
}

void KeyDown(int *param)
{
	int key=*(int*)param;

	float lambda=0.5f;
	bool ncam=false;

	switch(key)
	{
	case SDLK_w:	engine.curCamera->RelTranslate(vec3f(0,0, lambda));	break;
	case SDLK_s:	engine.curCamera->RelTranslate(vec3f(0,0,-lambda));	break;
	case SDLK_q:	engine.curCamera->RelTranslate(vec3f(0, lambda,0));	break;
	case SDLK_e:	engine.curCamera->RelTranslate(vec3f(0,-lambda,0));	break;
	case SDLK_a:	engine.curCamera->RelTranslate(vec3f( lambda,0,0));	break;
	case SDLK_d:	engine.curCamera->RelTranslate(vec3f(-lambda,0,0));	break;

	case SDLK_1: 	engine.curCamera=camera1;break;
	case SDLK_2:	engine.curCamera=camera2;break;

	case SDLK_ESCAPE:	engine.Stop();	break;

	default:
		printf("key with code %d pressed\n",key);
		break;
	}
}

void MouseMove(int *param)
{
	if(((int*)param)[4]>0)
	{
		float sensetivity=0.05f;
		engine.curCamera->RelRotate(vec3f(-((int*)param)[3]*sensetivity,((int*)param)[2]*sensetivity,0));
		engine.window->CenterCursor();
	}
}

void MouseDown(int *param)
{
	engine.window->CenterCursor();
	SDL_ShowCursor(SDL_FALSE);
}

void MouseUp(int *param)
{
	SDL_ShowCursor(SDL_TRUE);
}

int main(void)
{
	if(!engine.Init(AE_INIT_WINDOW|AE_INIT_RENDER_GL|AE_INIT_SCENE|AE_INIT_CAMERA))
		return 0;

	engine.Refresh=Refresh;
	engine.OnStart=StartInit;
	engine.OnKeyDown=KeyDown;
	engine.OnMouseMove=MouseMove;
	engine.OnMouseDown=MouseDown;
	engine.OnMouseUp=MouseUp;

	engine.Run();

	//puts("Join");
	//wnd.Join();
	//wnd.Stop();
	tracker.Stop();

	return 0;
}
