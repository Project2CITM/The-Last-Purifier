#ifndef __PARTICLE_ATTACK_REVENANT_H__
#define __PARTICLE_ATTACK_REVENANT_H__

#include "Particle.h"
#include "ModuleRender.h"

class ParticleAttackRevenant : public Particle
{
public:

	ParticleAttackRevenant(iPoint position,int rot = 0,  float life = 0, float delay = 0, bool hasPurifiedSword = false, iPoint velocity = { 0,0 }, std::string name = "Particle");

	~ParticleAttackRevenant();

	void PostUpdate() override;

};

#endif // !__PARTICLE_ATTACK_REVENANT_H__