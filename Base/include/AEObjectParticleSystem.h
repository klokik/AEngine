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
		Vec3f position;
		Vec3f velocity;
		Vec3f size;
		float life;			// in seconds

		AEParticle():
			position(vec3f(0,0,0)),
			velocity(vec3f(0,0,0)),
			size(vec3f(.1f,.1f,.1f)),
			life(0)
		{
		}

		AEParticle(Vec3f position,Vec3f velocity,Vec3f size,float life):
			position(position),velocity(velocity),size(size),life(life)
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

		std::random_device rnd_dev;
		std::mt19937 rgn;

		AEParticleEmitter():
			intensity(100.0f),initial_velocity(1),
			direction(vec3f(1.0f,0.0f,0.0f)),grouping(vec3f(0.0f,0.0f,0.0f)),
			rgn(rnd_dev())
		{
		}

		virtual AEParticle GetParticle(void);
	};

	class AEParticleAffector
	{
	public:
		// "true" if patricle survives, "false" if particle should die
		virtual bool Affect(AEParticle &particle,float dt_ms) = 0;
	};

	class AEParticleAffectorMove: public AEParticleAffector
	{
	public:
		virtual bool Affect(AEParticle &particle,float dt_ms);
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
			accel(vec3f(.0f,.0f,-_accel))
		{
		}

		virtual bool Affect(AEParticle &particle,float dt_ms);
	};

	class AEObjectParticleSystem: public AEObject
	{
	public:
		std::vector<AEParticle> particles;
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