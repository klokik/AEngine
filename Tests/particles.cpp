#include "AEEngine.h"
#include "AEObjectParticleSystem.h"
#include "AEResourceManager.h"


#include <memory>


using namespace aengine;

AEEngine engine;
AEObjectParticleSystem *part_sys;

void Refresh(int *param)
{
	part_sys->Update(param[0]);
}

void StartInit(int *param)
{
	engine.curCamera->SetTranslate(vec3f(0.0f,0.0f,10.0f));

	part_sys = new AEObjectParticleSystem;
	part_sys->material = engine.scene->materials.New();
	part_sys->material->texture = new AETexture;
	AEResourceManager::LoadImage("../resource/cloud.png",*part_sys->material->texture,"png");

	engine.scene->AddObject(part_sys);

	engine.render->CacheScene(engine.scene);
}

void KeyDown(int *param)
{
	int key=*(int*)param;

	switch(key)
	{
	case 'q':
		engine.Stop();
		break;
	default:
		break;
	}
}

int main(void)
{
	if(!engine.Init(AE_INIT_WINDOW|AE_INIT_RENDER_GL|AE_INIT_SCENE|AE_INIT_CAMERA))
		return 0;

	engine.Refresh=Refresh;
	engine.OnStart=StartInit;
	engine.OnKeyDown=KeyDown;
	// engine.OnMouseMove=MouseMove;
	// engine.OnMouseDown=MouseDown;
	// engine.OnMouseUp=MouseUp;

	engine.Run();

	return 0;
}
