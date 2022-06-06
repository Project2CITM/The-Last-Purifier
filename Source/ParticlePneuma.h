#pragma once
#include "Particle.h"

class ParticlePneuma :  public Particle
{
public:
	ParticlePneuma(iPoint position, int rot, float life, float delay = 0, iPoint velocity = { 0,0 }, std::string name = "PenumaParticle");
};

