#pragma once
#include "Particle.h"
class ParticleLanceAttackRevenant :
    public Particle
{
public:

	ParticleLanceAttackRevenant(iPoint position, int rot = 0, float life = 0, float delay = 0, bool hasPurifiedSword = false, SDL_RendererFlip flip = SDL_FLIP_NONE, iPoint velocity = { 0,0 }, std::string name = "Particle");

	~ParticleLanceAttackRevenant();
	
};

