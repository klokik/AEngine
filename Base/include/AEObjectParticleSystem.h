/*
 * AEObjectParticleSystem.h
 *
 *  Created on: Jan 04, 2014
 *      Author: klokik
 */

#ifndef AEOBJECTPARTICLESYSTEM_H
#define AEOBJECTPARTICLESYSTEM_H

#include <memory>
#include <random>

#include "AEObject.h"
#include "AEMaterial.h"
#include "AEVectorMath.h"


#define AE_OBJ_PARTICLE_SYSTEM 0x000A

namespace aengine
{
	class AEParticle;
	class AEObjectParticleSystem;
	class AEParticleEmitter;

	class AEParticle
	{
	public:
		Vec3f translate;
		Vec3f velocity;
		Vec3f size;
		float life;			// in seconds

		AEParticle():
			translate(vec3f(0,0,0)),
			velocity(vec3f(0,0,0)),
			size(vec3f(.1f,.1f,.1f)),
			life(0)
		{
		}

		AEParticle(Vec3f translate,Vec3f velocity,Vec3f size,float life):
			translate(translate),velocity(velocity),size(size),life(life)
		{
		}
	};

	class AEParticleEmitter
	{
	public:
		float intensity;
		float initial_velocity;
		Vec3f direction;
		Vec3f grouping;
		Vec3f translate;
		Vec3f particle_size;

		std::random_device rnd_dev;
		std::mt19937 rgn;

		AEParticleEmitter():
			intensity(100.0f),initial_velocity(1),
			direction(vec3f(0.0f,1.0f,0.0f)),grouping(vec3f(1.0f,0.0f,0.0f)),
			translate(vec3f(0,0,0)),particle_size(vec3f(0.5f,0.5f,0.5f)),
			rgn(rnd_dev())
		{
		}

		virtual AEParticle GetParticle(void);
	};

	class AEParticleAffector
	{
	public:
		// "true" if patricle survives, "false" if particle should die
		virtual bool Affect(AEParticle &particle,size_t pt_id,float dt_ms) = 0;

		enum class EventType {NEW_PARTICLE,PARTICLE_DIED};
		virtual void Notify(EventType event,AEParticle &particle,size_t pt_id) {};
	};

	class AEParticleAffectorMove: public AEParticleAffector
	{
	public:
		virtual bool Affect(AEParticle &particle,size_t pt_id,float dt_ms);
	};

	class AEParticleAffectorGravity: public AEParticleAffector
	{
	public:
		Vec3f accel;

		AEParticleAffectorGravity(Vec3f _accel):
			accel(_accel)
		{
		}

		AEParticleAffectorGravity(float _accel):
			accel(vec3f(.0f,-_accel,.0f))
		{
		}

		virtual bool Affect(AEParticle &particle,size_t pt_id,float dt_ms);
	};

	class AEParticleAffectorLifetime: public AEParticleAffector
	{
	private:
		std::map<size_t,float> attributes;

	public:
		float max_time;
		float range;

		AEParticleAffectorLifetime(float time,float range):
			max_time(time),range(range)
		{	
		}

		virtual bool Affect(AEParticle &particle,size_t pt_id,float dt_ms);

		virtual void Notify(EventType event,AEParticle &particle,size_t pt_id);
	};

	class AEObjectParticleSystem: public AEObject
	{
	public:
		std::map<size_t,AEParticle> particles;
		size_t nonce;
		AEParticleEmitter emitter;
		std::vector<std::shared_ptr<AEParticleAffector> > affectors;

		AEMaterial *material;

		float life_period;

		AEObjectParticleSystem();

		void Update(float dt_ms);

		// Emit amount of particles according to intensity
		void Emit(float dt_ms);

		// Emit specified amount
		void EmitNum(size_t amount);
	};
}

#endif /* AEOBJECTPARTICLESYSTEM_H */