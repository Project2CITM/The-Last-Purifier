#ifndef __PARTICLE_ATTACK_KABOOM_H__
#define __PARTICLE_ATTACK_KABOOM_H__

#include "Particle.h"
class ParticleAttackKaboom :public Particle
{
public: 
	ParticleAttackKaboom(iPoint position);

	~ParticleAttackKaboom();

	void PostUpdate() override;
};

#endif // !__PARTICLE_ATTACK_KABOOM_H__