#pragma once
#include "Particle.h"

class ParticlePurificationSlash :
    public Particle
{
public:

	ParticlePurificationSlash(iPoint position, int rot = 0, float life = 0, float delay = 0, bool hasPurifiedSword = false, iPoint velocity = { 0,0 }, std::string name = "Particle");

	~ParticlePurificationSlash();

};

