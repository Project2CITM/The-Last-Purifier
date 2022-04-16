#pragma once
#include "Particle.h"
#include "ModuleRender.h"

class ParticleAttackRevenant :
    public Particle
{
public:

	ParticleAttackRevenant(iPoint position, float life = 0, float delay = 0, iPoint velocity = { 0,0 }, std::string name = "Particle");

	~ParticleAttackRevenant();

};

