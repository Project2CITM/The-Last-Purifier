#ifndef __PARTICLE_HIT_SAGE_H__
#define __PARTICLE_HIT_SAGE_H__

#include "Particle.h"
#include "RenderObject.hpp"

class ParticleHitSage : public Particle
{
public:

	ParticleHitSage(iPoint position, float life = 0, float delay = 0, iPoint velocity = { 0,0 }, std::string name = "Particle");

	~ParticleHitSage();
};

#endif // !__PARTICLE_HIT_SAGE_H__