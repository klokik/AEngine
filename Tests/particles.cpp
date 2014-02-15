#include "AEEngine.h"
#include "AEObjectParticleSystem.h"
#include "AEResourceManager.h"


#include <memory>


using namespace aengine;

AEEngine engine;
AEObjectParticleSystem *part_sys;
AEObjectParticleSystem *part_sys2;

void InitFire()
{
	part_sys = new AEObjectParticleSystem;
	part_sys->affectors.push_back(std::make_shared<AEParticleAffectorGravity>(-3.f));
	part_sys->affectors.push_back(std::make_shared<AEParticleAffectorLifetime>(2.f,.0f));
	part_sys->affectors.push_back(std::make_shared<AEParticleAffectorFunctional>(
		[](AEParticle &particle,size_t pt_id,float dt_ms)->bool{
			particle.velocity=particle.velocity+vec3f(sin(pt_id%311*particle.time),0.f,cos(static_cast<int>(pow(pt_id,569))%23*particle.time))*0.2f;
			return true;}));
	// size
	part_sys->affectors.push_back(std::make_shared<AEParticleAffectorFunctional>(
		[](AEParticle &particle,size_t pt_id,float dt_ms)->bool{
			particle.size = vec3f(1,1,1)*sqrt(-particle.time+3);
			return true;}));
	// color
	part_sys->affectors.push_back(std::make_shared<AEParticleAffectorFunctional>(
		[](AEParticle &particle,size_t pt_id,float dt_ms)->bool{
			float fs = sin(particle.time*3.1415926f/2/3);
			particle.color.vec = vec4f(0,0,1,1)*(1-fs)+vec4f(1,1,1,1)*fs;
			return true;}));
	part_sys->material = engine.scene->materials.New();
	part_sys->material->texture = new AETexture;
	part_sys->emitter.translate = vec3f(-5,-5.0f,0);
	part_sys->emitter.grouping = vec3f(1.0f,0,0);
	AEResourceManager::LoadImage("../resource/fireball.png",*part_sys->material->texture,"png");
}

void InitBullets()
{
	part_sys2 = new AEObjectParticleSystem;

	part_sys2->emitter.translate = vec3f(5,0,0);
	part_sys2->emitter.direction = vec3f(1,0.2f,0);
	part_sys2->emitter.initial_velocity = 43.0f;
	part_sys2->emitter.grouping = vec3f(0,0,0);

	part_sys2->affectors.push_back(std::make_shared<AEParticleAffectorLifetime>(.5f,.0f));
}

void Refresh(int *param)
{
	part_sys->EmitNum(1);
	part_sys->Update(param[0]);
	part_sys2->EmitNum(1);
	part_sys2->Update(param[0]);
}

void StartInit(int *param)
{
	engine.curCamera->SetTranslate(vec3f(0.0f,0.0f,10.0f));

	InitFire();
	InitBullets();

	engine.scene->AddObject(part_sys);
	engine.scene->AddObject(part_sys2);

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
