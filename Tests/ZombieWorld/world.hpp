#include <vector>
#include <cmath>

#include "debug.hpp"
#include "AEEngine.h"
#include "AEObjectMesh.h"
#include "AEObjectEmpty.h"
#include "AEObjectCamera.h"
#include "AEVectorMath.h"
#include "AEObjectText.h"

#include "graph.hpp"

void LoadObjFile(AEMesh *&mesh, const char *path);

namespace Zombie
{
	using namespace std;
	using namespace aengine;

	class ZombieWorld;

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

		ZombieWorld *world;

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
		void run(float power);
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

	class Chunk: public AEObjectEmpty
	{
	public:
		int id;
		vector<Mob*> mobs;
		int X,Y;

		Graph graph;

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
		vector<Chunk*> chunks;

		vector<Mob*> mobs;

		AEMesh *player_mesh;
		AEMesh *zombie_mesh;
		Mob *player;

		int seed;

		Vec3f cam_offset;

		AEEngine engine;

		AEObjectMesh *chunk_mesh;
		AEObjectText *otext;

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

			engine.scene->fonts.LoadFont("../resource/font1.png","font1",16,16);

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

			// for(int q=0;q<10;q++)
			// {
			// 	Mob *mob = createMob(Mob::ZOMBIE,vec3f(q-5,0.1f,20.0f));
			// 	mob->motivation = 0.1f*(q+1);
			// 	engine.scene->AddObject(mob);
			// }

			engine.scene->AddObject(this->player);
			engine.scene->AddObject(chunk_mesh);

			otext = new AEObjectText;
			otext->alignment = AE_LEFT;
			otext->text = "OMG";
			otext->SetTranslate(vec3f(25.0f,25.0f,0.0f));
			engine.scene->AddObject(otext);

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

			getChunk(vec2f(player->translate.X,player->translate.Z))->update(ms);

			otext->text = "";
			for(size_t q=0;q<player->inventory.size();q++)
			{
				Item item = player->inventory[q];
				otext->text+=item.name + ":" + std::to_string(item.amount) + " ";
			}
		}

		Chunk *getChunk(int x,int y)
		{
			int nx = nearbyint(x/100.0f)*100;
			int ny = nearbyint(y/100.0f)*100;
			for(auto chunk:chunks)
			{
				if(chunk->X==nx&&chunk->Y==ny)
					return chunk;
			}

			loadChunk(nx,ny);

			return getChunk(nx,ny);
		}

		Chunk *getChunk(Vec2f pos)
		{
			return getChunk((int)floor(pos.X),(int)floor(pos.Y));
		}

		void generateChunk(int x,int y)
		{
			Chunk *chunk = new Chunk;
			chunk->X = x;
			chunk->Y = y;

			chunk->graph.loadFromFile("../resource/graph.txt");

			AEObjectMesh *obj_mesh = new AEObjectMesh;
			obj_mesh->mesh = chunk_mesh->mesh;
			obj_mesh->material = chunk_mesh->material;

			chunk->AddChild(obj_mesh);

			chunk->SetTranslate(vec3f(x,0.0f,y));
			engine.scene->AddObject(chunk);

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

		Item item;
		item.name = "Bread";
		item.amount = 2;
		mob->inventory.push_back(item);
		item.name = "Pepsi";
		item.amount = 3;
		mob->inventory.push_back(item);

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

	void Mob::update(ZombieWorld &_world)
	{
		this->world = &_world;
		if(mob_type==PLAYER)
		{
			if(
				_world.engine.GetKeyState('w')||
				_world.engine.GetKeyState('s')||
				_world.engine.GetKeyState('a')||
				_world.engine.GetKeyState('d'))
					run(2.0f);
		}

		if(mob_type==ZOMBIE)
		{
			setTarget(*_world.player);
			orient();
			run(motivation);
		}
	}

	void Mob::run(float power)
	{
		float delta=0.1f;
		Move(vec3f(0,0,-power*delta));

		Chunk *chunk = world->getChunk(translate.X,translate.Z);
		Graph::TestResult result;
		result = chunk->graph.test(translate,vec3f(chunk->X,0,chunk->Y));

		if(result.passed)
		{
			//set altitude
			_translate.Y = result.altitude;		//FIXME: bug
			InvalidateTranslate();
		}
		else
			Move(vec3f(0,0,power*delta)); //move back
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