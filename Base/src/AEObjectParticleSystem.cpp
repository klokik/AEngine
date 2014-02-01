/*
 * AEObjectParticleSystem.cpp
 *
 *  Created on: Jan 04, 2014
 *      Author: klokik
 */

#include "AEObjectParticleSystem.h"


namespace aengine
{
	AEParticle AEParticleEmitter::GetParticle(void)
	{
		AEParticle item;

		item.velocity=normalize(vec3f(	std::generate_canonical<float,10>(rgn)-0.5f,
										std::generate_canonical<float,10>(rgn)-0.5f,
										std::generate_canonical<float,10>(rgn)-0.5f))*initial_velocity;

		return item;
	}

	AEObjectParticleSystem::AEObjectParticleSystem(void):
		material(nullptr),life_period(5)
	{
		_type=AE_OBJ_PARTICLE_SYSTEM;
		affectors.push_back(std::make_shared<AEParticleAffectorMove>());
	}

	void AEObjectParticleSystem::Update(float dt_ms)
	{
		//particles.push_back(emitter.GetParticle());

		for(auto &affector:affectors)
		{
			for(AEParticle &item:particles)
			{
				if(!affector->Affect(item,dt_ms));
					//destroy particle

				// item.life+=dt_ms*0.001f;
				// if(item.life>=life_period)
				// {
				// 	// Destroy
				// 	item.position=vec3f(0,0,0);
				// 	item.life=0;
				// }
			}
		}
	}

	void AEObjectParticleSystem::Emit(float dt_ms)
	{
		throw "not implemented";
	}

	void AEObjectParticleSystem::EmitNum(size_t amount)
	{
		for(size_t q=0;q<amount;q++)
			particles.push_back(emitter.GetParticle());
	}

	bool AEParticleAffectorMove::Affect(AEParticle &particle,float dt_ms)
	{
		particle.position=particle.position+particle.velocity*dt_ms*0.001f;

		return true;
	}

	bool AEParticleAffectorGravity::Affect(AEParticle &particle,float dt_ms)
	{
		particle.velocity=particle.velocity+accel*dt_ms*0.001f;

		return true;
	}
}