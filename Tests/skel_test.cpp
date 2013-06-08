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
#include "AEObjectMesh.h"
#include "AEObjectSprite.h"
#include "AEObjectEmpty.h"
#include "AEObjectText.h"
#include "AEObjectJoint.h"


using namespace aengine;
using namespace std;

void LoadObjFile(AEMesh *&mesh, const char *path);

AEEngine engine;
AEObjectCamera *camera1,*camera2;
AEObjectMesh *pobj,*plane;
AEObjectLight *plight1;
AEObjectJoint *skeleton;

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

	skeleton->RelRotate(vec3f(0.0f,0.0f,1.0f));
	skeleton->children[0]->RelRotate(vec3f(0.0f,1.0f,0.0f));

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

	camera1->SetTranslate(vec3f(0,5,0));
	plight1=new AEObjectLight(AE_AMBIENT_LIGHT);
	plight1->color.A=0.5f;

	pobj=new AEObjectMesh;
	// pobj->SetTranslate(vec3f(0,0,-10));
	plane=new AEObjectMesh;

	AEMesh *cube_mesh = new AEMesh;
	AEMesh *plane_mesh = new AEMesh;

	LoadObjFile(cube_mesh,"../resource/cube_flat.obj");
	LoadObjFile(plane_mesh,"../resource/landskape_texmtn.obj");

	pobj->SetMesh(cube_mesh);
	plane->SetMesh(plane_mesh);

	AEMaterial *mat=engine.scene->materials.New();
	engine.scene->materials.LoadTexture(mat->texture,"../resource/grass.jpg");
	//pobj->material=mat;
	plane->material=mat;

	skeleton = new AEObjectJoint;
	skeleton->SetTranslate(vec3f(0.0f,5.0f,-10.0f));
	skeleton->display_color = {1.0f,1.0f,0.0f};
	skeleton->SetTail(vec3f(0.0f,5.0f,0.0f));
	AEObjectJoint * arm = new AEObjectJoint;
	arm->SetTail(vec3f(3.0f,0.0f,0.0f));
	arm->display_color = {0.0f,1.0f,0.0f};
	// arm->SetTranslate(vec3f(5.0f,0.0f,0.0f));
	skeleton->AddChild(arm);

	//*--region skin

	AEVertexGroup cube_group(pobj);
	for(size_t q=0;q<pobj->mesh->vtxcount;q++)
		cube_group.AddVertex(q,1.0f);
	arm->vertex_groups.push_back(cube_group);
	// arm->AddChild(pobj);

	//*/// endregion skin

	engine.scene->AddObject(pobj);
	engine.scene->AddObject(camera2);
	engine.scene->AddObject(plight1);
	engine.scene->AddObject(plane);
	engine.scene->AddObject(skeleton);

	// AECOLLADALoader loader;
	// loader.Load(engine.scene,"resource/town.dae");

	engine.render->CacheScene(engine.scene);

	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	//SDL_EnableKeyRepeat(10,20);
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

void PrintMatrix(AEMatrix4f4 &mtx)
{
	for(int q=0;q<16;q++)
	{
		cout<<mtx[q]<<"\t";
		if((q+1)%4==0)
			cout<<endl;
	}
}
void MatrixTest(void)
{
	AEMatrix4f4 A;
	AEMatrix4f4 B;
	AEMatrix4f4 C;

	cout<<"A:\n";
	PrintMatrix(A);
	cout<<"A*B:\n";
	C = A*B;
	PrintMatrix(C);

	A.Translate(1,2,3);
	cout<<"Translated A:\n";
	PrintMatrix(A);
	cout<<"A*B:\n";
	C = A*B;
	PrintMatrix(C);
	B.RotateX(60.0f);
	cout<<"Rotated B:\n";
	PrintMatrix(B);
}

int main(void)
{
	// MatrixTest();
	// return 0;

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
