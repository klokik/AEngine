#include <vector>
#include <cmath>

#include "debug.hpp"
#include "AEEngine.h"
#include "AEObjectMesh.h"
#include "AEObjectEmpty.h"
#include "AEObjectCamera.h"
#include "AEVectorMath.h"

void LoadObjFile(AEMesh *&mesh, const char *path);

namespace Zombie
{
	using namespace std;
	using namespace aengine;

	class Mob;
	class ZombieWorld;

	class Item
	{
	public:
		enum ItemType {WEAPON,FOOD,TOOL};
		enum ItemState {ACTIVE=1,INACTIVE=2,BROKEN=4};

		ItemType item_type;
		ItemState state;

		string name;
		float value;
		float health;


		void apply(Mob &mob);

		void activate();
		void deactivate();
		void repair();
	};

	class Mob: public AEObjectMesh
	{
	public:
		int id;
		float speed;
		float max_speed;

		enum MobType {PLAYER,CAR,ZOMBIE,BUILDING,CHEST,SKELETON,DOG};
		enum MobState {ACTIVE,PASSIVE};

		MobType mob_type;
		MobState mob_state;

		float view_radius;
		float hear_radius;
		float smell_radius;
		float fov;

		float health;
		float hunger;
		float water;
		float motivation;

		vector<Item> inventory;

		enum Target {POINT,MOB,NO_TARGET};

		Mob *targ_mob;
		Vec3f targ_pos;
		Target targ_type;

		Mob(void)
		{
			motivation = 0.5f;
			targ_mob = NULL;
			targ_type = NO_TARGET;
		}
		void run(float power)
		{
			float delta=0.1f;
			Move(vec3f(0,0,-power*delta));
		}
		void attack();
		void jump(float power)
		{

		}

		void setTarget(Vec3f position)
		{
			targ_pos = position;
			targ_type = POINT;
		}

		void setTarget(Mob &mob)
		{
			targ_mob = &mob;
			targ_type = MOB;
		}

		void tracePath();

		void orient(float angle)
		{
			this->SetRotate(vec3f(0,angle,0));	
		}

		void orient()
		{
			switch(targ_type)
			{
			case NO_TARGET:
				break;
			case MOB:
				if(!targ_mob)
					break;
				targ_pos = targ_mob->translate;
			case POINT:
				{
					static float pi = 3.14159265f;
					float angle;
					Vec3f delta = targ_pos - translate;
					angle = -(90.0f - acos(delta.X/Length(delta))*180.0f/pi);
					if(delta.Z>0)
						angle = 180 - angle;

					orient(angle);
				}
				break;
			}
		}

		void update(ZombieWorld &world);
	};

	class Chunk
	{
	public:
		int id;
		vector<Mob*> mobs;
		int X,Y;

		void update(float ms)
		{
			//TODO
		}
	};

	ZombieWorld *current_world=NULL;

	void startInit(int *param);
	void refreshEng(int *param);
	void mouseAction(int *param);
	void keyDown(int *param);
	void keyUp(int *param);

	class ZombieWorld
	{
	public:
		vector<Chunk> chunks;

		vector<Mob*> mobs;

		AEMesh *player_mesh;
		AEMesh *zombie_mesh;
		Mob *player;

		int seed;

		Vec3f cam_offset;

		AEEngine engine;

		AEObjectMesh *chunk_mesh;

		// ------------------------ Life cycle -------------------------
		void generate(int seed,string name)
		{

		}
		void loadState(string name);
		void saveState(string name)
		{
			debugMsg("Saving state unsupported!");
		}

		void start(void)
		{
			debugMsg("Firing up the Engine");

			if(!engine.Init(AE_INIT_WINDOW|AE_INIT_RENDER_GL|AE_INIT_SCENE|AE_INIT_CAMERA))
				throw new ZombieException(0,"Failed to init engine");

			current_world = this;

			engine.Refresh = refreshEng;
			engine.OnMouseMove = mouseAction;
			engine.OnStart = startInit;
			engine.OnKeyDown = keyDown;
			engine.OnKeyUp = keyUp;

			engine.Run();
		}

		void stop()
		{
			debugMsg("Stop msg was sent");
			engine.Stop();
		}

		int waitEnd(void)
		{
			debugMsg("Join to the engine process");
			debugMsg("Still not implemented");
			// throw new ZombieException(NOT_IMPLEMENTED,"Not Implemented");

			return 0;
		}

		// -------------------------------------------------------------

		Mob *createMob(Mob::MobType type,Vec3f pos);

		void engineStarted(void)
		{
			debugMsg("Engine started");

			AEObjectCamera *camera = new AEObjectCamera;

			cam_offset = vec3f(0.0f,50.0f,25.0f);
			camera->SetTranslate(cam_offset);
			camera->SetRotate(vec3f(-60.0f,0.0f,0.0f));

			//player->AddChild(camera);

			engine.curCamera=camera;

			chunk_mesh = new AEObjectMesh;
			LoadObjFile(chunk_mesh->mesh,"../resource/square.obj");
			chunk_mesh->material = engine.scene->materials.New();
			engine.scene->materials.LoadTexture(chunk_mesh->material->texture,"../resource/checker.png");

			LoadObjFile(player_mesh,"../resource/player.obj");
			zombie_mesh = player_mesh;

			player = createMob(Mob::PLAYER,vec3f(0.0f,0.0f,0.5f));

			for(int q=0;q<10;q++)
			{
				Mob *mob = createMob(Mob::ZOMBIE,vec3f(q-5,0.1f,20.0f));
				mob->motivation = 0.1f*(q+1);
				engine.scene->AddObject(mob);
			}

			engine.scene->AddObject(this->player);
			engine.scene->AddObject(chunk_mesh);

			engine.render->CacheScene(engine.scene);
		}

		void updateWorld(float ms)
		{
			static float delta = 0;
			static int frames = 0;
			delta += ms;
			frames++;

			if(delta>=5000)
			{
				cout<<"FPS: "<<frames*1000/delta<<endl;
				delta = 0;
				frames = 0;
			}


			engine.curCamera->SetTranslate(player->translate+cam_offset);

			for(size_t q=0;q<mobs.size();q++)
				mobs[q]->update(*this);

			getChunk(vec2f(player->translate.X,player->translate.Z)).update(ms);

		}

		Chunk &getChunk(int x,int y)
		{
			for(auto chunk:chunks)
			{
				if(chunk.X==x&&chunk.Y==y)
					return chunk;
			}

			loadChunk(x,y);

			return getChunk(x,y);
		}

		Chunk &getChunk(Vec2f pos)
		{
			return getChunk((int)floor(pos.X),(int)floor(pos.Y));
		}

		void generateChunk(int x,int y)
		{
			Chunk chunk;
			chunk.X = x;
			chunk.Y = y;

			chunks.push_back(chunk);
		}

		void loadChunk(int x,int y)
		{
			//TODO: try to load chunk from file

			generateChunk(x,y);
		}

		// -----------------------------------------------------------

		void keyboardInput(int keycode,int down)
		{
			if(!down)
				return;
			
			switch(keycode)
			{
			case 'w':
				player->orient(0.0f);
			//	player->run(1.0f);
				break;
			case 's':
				player->orient(180.0f);
			//	player->run(1.0f);
				break;
			case 'a':
				player->orient(90.0f);
			//	player->run(1.0f);
				break;
			case 'd':
				player->orient(270.0f);
			//	player->run(1.0f);
				break;
			case ' ':
				player->jump(1.0f);
				break;

			case 27:
				stop();
				break;

			default:
				break;
			}
		}
	};

	Mob *ZombieWorld::createMob(Mob::MobType type,Vec3f pos)
	{
		Mob *mob = new Mob;
		mob->SetTranslate(pos);
		mob->mob_type = type;
		mob->mob_state = Mob::ACTIVE;

		// Set appropriate appearence
		switch(type)
		{
		case Mob::PLAYER:
			mob->mesh = player_mesh;
			break;
		case Mob::ZOMBIE:
			mob->mesh = zombie_mesh;
			break;
		default:
			break;
		}

		mobs.push_back(mob);

		return mob;
	}

	void Mob::update(ZombieWorld &world)
	{
		if(mob_type==PLAYER)
		{
			if(
				world.engine.GetKeyState('w')||
				world.engine.GetKeyState('s')||
				world.engine.GetKeyState('a')||
				world.engine.GetKeyState('d'))
					run(2.0f);
		}

		if(mob_type==ZOMBIE)
		{
			setTarget(*world.player);
			orient();
			run(motivation);
		}
	}

	void startInit(int *param)
	{
		if(current_world==NULL)
			throw new ZombieException(0,"Where is the world!?");

		current_world->engineStarted();
	}

	void refreshEng(int *param)
	{
		if(current_world==NULL)
			throw new ZombieException(0,"Where is the world!?");

		current_world->updateWorld(param[0]);
	}

	void mouseAction(int *param)
	{
		//throw new ZombieException(NOT_IMPLEMENTED,"Not impl.");
	}

	void keyDown(int *param)
	{
		if(current_world==NULL)
			throw new ZombieException(0,"Where is the world!?");

		current_world->keyboardInput(param[0],1);
	}

	void keyUp(int *param)
	{
		if(current_world==NULL)
			throw new ZombieException(0,"Where is the world!?");

		current_world->keyboardInput(param[0],0);
	}
}