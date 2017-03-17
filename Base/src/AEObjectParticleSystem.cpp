/*
 * AEObjectParticleSystem.cpp
 *
 *  Created on: Jan 04, 2014
 *      Author: klokik
 */

#include <stdexcept>

#include "AEObjectParticleSystem.h"
#include "AEDebug.h"


namespace aengine
{
	AEParticle AEParticleEmitter::GetParticle(void)
	{
		AEParticle item;

		auto rv2=vec2f(
			std::generate_canonical<float,10>(rgn),
			std::generate_canonical<float,10>(rgn))*2-vec2f(1,1);

		auto rand_vec=grouping*rv2.X + cross(grouping,direction)*rv2.Y;

		item.velocity=normalize(direction+rand_vec)*initial_velocity;

		item.translate=translate;
		item.size=particle_size;

		return item;
	}

	AEObjectParticleSystem::AEObjectParticleSystem(void):
		nonce(0),material(nullptr),life_period(5)
	{
		_type=AE_OBJ_PARTICLE_SYSTEM;
		affectors.push_back(std::make_shared<AEParticleAffectorMove>());
	}

	void AEObjectParticleSystem::Update(float dt_ms)
	{
		//particles.push_back(emitter.GetParticle());
		std::vector<size_t> to_erase;

		for(auto &affector:affectors)
		{
			size_t i=0;
			for(auto &item:particles)
			{
				if(!affector->Affect(item.second,item.first,dt_ms))
					to_erase.push_back(item.first);
				i++;
			}
		}

		for(auto item:to_erase)
		{
			for(auto &affector:affectors)
				affector->Notify(AEParticleAffector::EventType::PARTICLE_DIED,particles[item],item);

			particles.erase(item);
		}
	}

	void AEObjectParticleSystem::Emit(float dt_ms)
	{
		throw "not implemented";
	}

	void AEObjectParticleSystem::EmitNum(size_t amount)
	{
		for(size_t q=0;q<amount;q++)
		{
			auto particle=emitter.GetParticle();
			particles.emplace(nonce,particle);
			for(auto &affector:affectors)
				affector->Notify(AEParticleAffector::EventType::NEW_PARTICLE,particle,nonce);
			nonce++;
		}
	}

	void AEObjectParticleSystem::EmitNum(size_t amount,std::vector<void*> ex)
	{
		for(size_t q=0;q<amount;q++)
		{
			auto particle=emitter.GetParticle();
			particles.emplace(nonce,particle);
			for(auto &affector:affectors)
				affector->Notify(AEParticleAffector::EventType::NEW_PARTICLE,particle,nonce,ex[q]);
			nonce++;
		}
	}

	bool AEParticleAffectorMove::Affect(AEParticle &particle,size_t pt_id,float dt_ms)
	{
		particle.translate=particle.translate+particle.velocity*dt_ms*0.001f;

		return true;
	}

	bool AEParticleAffectorGravity::Affect(AEParticle &particle,size_t pt_id,float dt_ms)
	{
		particle.velocity=particle.velocity+accel*dt_ms*0.001f;

		return true;
	}

	bool AEParticleAffectorLifetime::Affect(AEParticle &particle,size_t pt_id,float dt_ms)
	{
		auto &lifetime = particle.time;

		lifetime+=dt_ms*0.001f;

		if(lifetime>max_time)
			return false;
		return true;
	}
}