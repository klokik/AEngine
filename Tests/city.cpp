/*
 * town.cpp
 *
 *  Created on: Apr 24, 2013
 *      Author: klokik
 */

#include <stdio.h>
#include <math.h>
#include <GL/gl.h>

#include <iostream>
#include <stdlib.h>

#include <SDL/SDL.h>

#include "AEEngine.h"

#include "AETexture.h"
#include "AETimeline.h"
#include "AEVectorMath.h"
#include "Utils/AEUBitMapTextGenerator.h"
#include "AEObjectMesh.h"
#include "AEObjectSprite.h"
#include "AEObjectEmpty.h"
#include "AEObjectText.h"
#include "AEResourceManager.h"

#include "AECOLLADALoader.h"

//#include "AEMatrix4f4.h"

using namespace aengine;

#define p_size 100

AEObjectCamera *camera1,*camera2;
AEEngine engine;
AEObjectMesh *ground;
AEObjectSprite *sprite,*sprite1;
AEObjectEmpty *empty[p_size*p_size];
AEObjectLight *plight1,*plight2,*plightx;
AEObjectText *otext;

float cam_x;
float cam_z;
float cam_y;


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

	if(engine.GetKeyState(SDLK_g))	plightx->RelTranslate(vec3f(0,-lambda,0));
	if(engine.GetKeyState(SDLK_t))	plightx->RelTranslate(vec3f(0, lambda,0));
	if(engine.GetKeyState(SDLK_h))	plightx->RelTranslate(vec3f( lambda,0,0));
	if(engine.GetKeyState(SDLK_f))	plightx->RelTranslate(vec3f(-lambda,0,0));
	if(engine.GetKeyState(SDLK_y))	plightx->RelTranslate(vec3f(0,0,-lambda));
	if(engine.GetKeyState(SDLK_r))	plightx->RelTranslate(vec3f(0,0, lambda));

	if(engine.GetKeyState(SDLK_u))	plightx->RelRotate(vec3f( mlambda,0,0));
	if(engine.GetKeyState(SDLK_j))	plightx->RelRotate(vec3f(-mlambda,0,0));
	if(engine.GetKeyState(SDLK_i))	plightx->RelRotate(vec3f(0, mlambda,0));
	if(engine.GetKeyState(SDLK_k))	plightx->RelRotate(vec3f(0,-mlambda,0));
	if(engine.GetKeyState(SDLK_o))	plightx->RelRotate(vec3f(0,0, mlambda));
	if(engine.GetKeyState(SDLK_l))	plightx->RelRotate(vec3f(0,0,-mlambda));

	if(engine.GetKeyState(SDLK_PAGEUP)) plight2->color.A+=0.5f;
	if(engine.GetKeyState(SDLK_PAGEDOWN)) plight2->color.A-=0.5f;

	// if(engine.GetKeyState(SDLK_z))	sol->RelScale(vec3f(0, mlambda/3,0));
	// if(engine.GetKeyState(SDLK_x))	sol->RelScale(vec3f(0,-mlambda/3,0));

	// //pobj->rotate.Y+=1;
	// engine.scene->objects[4]->RelRotate(vec3f(1.7f,0,0));
	// engine.scene->objects[4]->RelRotate(vec3f(0,0.7f,0));

	// engine.curCamera->SetTranslate(vec3f(cam_x,cam_y,cam_z));

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
	char buf[32];
	sprintf(buf,"frame: %d",frame);
	otext->text=buf;
}

void GenerateCity(void)
{
	AEMesh *tcube=new AEMesh;
	AEResourceManager::LoadMesh("../resource/cube_flat.obj",*tcube);//

	ground = new AEObjectMesh;
	ground->mesh = tcube;
	ground->SetScale(vec3f(100.0f,1.0f,100.0f));
	ground->SetTranslate(vec3f(0.0f,-0.5f,0.0f));
	ground->material=engine.scene->materials.New();
	ground->material->diffuse={0.7f,0.7f,0.7f,1.0f};
	engine.scene->AddObject(ground);

	AEMaterial *build_mat = engine.scene->materials.New();
	build_mat->diffuse={1.0f,1.0f,1.0f,1.0f};


	for(int q=0;q<p_size;q++)
	{
		AEObjectMesh *building = new AEObjectMesh;
		building->mesh = tcube;
		building->material = build_mat;

		AEVector3f scale = vec3f(rand()%9+1,rand()%19+1,rand()%9+1);

		building->SetScale(scale);
		building->SetTranslate(vec3f(rand()%200-100,scale.Y/2,rand()%200-100));

		engine.scene->AddObject(building);
	}

	// Set camera animatios

	AETimeline *camera_x = engine.scene->animations.Add(&cam_x,100.0f,true);
	AETimeline *camera_y = engine.scene->animations.Add(&cam_y,100.0f,true);
	AETimeline *camera_z = engine.scene->animations.Add(&cam_z,100.0f,true);
	for(int q=0;q<9;q++)
	{
		camera_x->SetKey(10.0f*q,(rand()%20-10)*10.0f);
		camera_y->SetKey(10.0f*q,(rand()%40+10));
		camera_z->SetKey(10.0f*q,(rand()%20-10)*10.0f);
	}
}

void StartInit(int *)
{
	camera1=engine.curCamera;
	camera1->SetTranslate(vec3f(0.0f,5.0f,0.0f));
	camera2=new AEObjectCamera;

	plightx=plight1=new AEObjectLight(AE_AMBIENT_LIGHT);
	plight2 = new AEObjectLight(AE_POINT_LIGHT);
	//plight2->SetRotate(vec3f(135.0f,30.0f,0.0f));

	plight1->color.A=0.1f;
	plight2->color.A=0.9f;


	GenerateCity();
	// sol->material=engine.scene->materials.New();
	// sol->material->diffuse={1.0f,1.0f,0.0f,1.0f};
	// sol->material->shading=false;
	// mercury->material=engine.scene->materials.New();
	// mercury->material->diffuse={0.6f,0.15f,0.1f,1.0f};
	// earth->material=engine.scene->materials.New();
	// earth->material->diffuse={0.2f,0.2f,0.8f,1.0f};

	// AEMaterial *mat=engine.scene->materials.New();
	// engine.scene->materials.LoadTexture(mat->texture,"../resource/grass.jpg");
	// pobj->material=mat;

	engine.scene->fonts.LoadFont("../resource/font1.png","font1",16,16);

	otext = new AEObjectText;
	otext->text="hello, world!";
	otext->char_alignment=AE_LEFT;
	otext->SetTranslate(vec3f(25.0f,260.0f,0.0f));

	engine.scene->AddObject(plight1);
	engine.scene->AddObject(plight2);
	// engine.scene->AddObject(otext);

	// AECOLLADALoader loader;
	// loader.Load(engine.scene,"../resource/lights.dae");

	engine.render->CacheScene(engine.scene);

	// glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	// SDL_EnableKeyRepeat(10,20);
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

	case SDLK_3: 	plightx=plight1;break;
	case SDLK_4:	plightx=plight2;break;

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
		engine.curCamera->RelRotate(vec3f(((int*)param)[3]*sensetivity,((int*)param)[2]*sensetivity,0));
		engine.window->CenterCursor();
	}

	//sprite->SetTranslate({(float)((int*)param)[0],(float)((int*)param)[1],0.0f});
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

	return 0;
}
