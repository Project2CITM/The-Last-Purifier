#pragma once
#include "Particle.h"
class ParticleExplodeEkriksi :
    public Particle
{
public:
    ParticleExplodeEkriksi(iPoint position, float life, float delay = 0, iPoint velocity = { 0,0 }, std::string name = "EkriskiParticle");

    ~ParticleExplodeEkriksi();
};

