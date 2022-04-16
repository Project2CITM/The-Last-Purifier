#pragma once
#include "Particle.h"

class ParticleAttackSage : 
	public Particle
{
public:

	ParticleAttackSage(iPoint position, float life = 0, float delay = 0, iPoint velocity = { 0,0 }, std::string name = "Particle");

	~ParticleAttackSage();
public:
	
};

