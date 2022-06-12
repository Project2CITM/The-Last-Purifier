#ifndef __PARTICLE_ATTACK_KABOOM_H__
#define __PARTICLE_ATTACK_KABOOM_H__

#include "Particle.h"
class ParticleAttackKaboom :public Particle
{
public: 
	ParticleAttackKaboom(iPoint position);

	~ParticleAttackKaboom();

	void PreUpdate() override;

	void PostUpdate() override;

private:

	uint explodeSFX;
};

#endif // !__PARTICLE_ATTACK_KABOOM_H__