#ifndef __PARTICLE_PURIFICATION_SLASH_H__
#define __PARTICLE_PURIFICATION_SLASH_H__

#include "Particle.h"

class ParticlePurificationSlash : public Particle
{
public:

	ParticlePurificationSlash(iPoint position, int rot = 0, float life = 0, float delay = 0, bool hasPurifiedSword = false, iPoint velocity = { 0,0 }, std::string name = "Particle");

	~ParticlePurificationSlash();
};

#endif // !__PARTICLE_PURIFICATION_SLASH_H__