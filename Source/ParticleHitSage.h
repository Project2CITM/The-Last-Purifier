#pragma once
#include "Particle.h"
#include "RenderObject.hpp"

class ParticleHitSage :
    public Particle
{
public:

	ParticleHitSage(iPoint position, float life = 0, float delay = 0, iPoint velocity = { 0,0 }, std::string name = "Particle");

	~ParticleHitSage();
public:

};

