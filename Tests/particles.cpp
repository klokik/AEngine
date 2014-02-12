#include "AEEngine.h"
#include "AEObjectParticleSystem.h"
#include "AEResourceManager.h"


#include <memory>


using namespace aengine;

AEEngine engine;
AEObjectParticleSystem *part_sys;

void Refresh(int *param)
{
	part_sys->EmitNum(1);
	part_sys->Update(param[0]);
}

void StartInit(int *param)
{
	engine.curCamera->SetTranslate(vec3f(0.0f,0.0f,10.0f));

	part_sys = new AEObjectParticleSystem;
	part_sys->affectors.push_back(std::make_shared<AEParticleAffectorGravity>(-5.f));
	part_sys->affectors.push_back(std::make_shared<AEParticleAffectorLifetime>(2.f,.0f));
	part_sys->material = engine.scene->materials.New();
	part_sys->material->texture = new AETexture;
	part_sys->emitter.translate = vec3f(0,-5.0f,0);
	part_sys->emitter.grouping = vec3f(3.0f,0,0);
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
