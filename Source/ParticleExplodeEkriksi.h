#pragma once
#include "Particle.h"
class ParticleExplodeEkriksi :
    public Particle
{
    ParticleExplodeEkriksi(iPoint position, float life, float delay, iPoint velocity, std::string name);

    ~ParticleExplodeEkriksi();
};

