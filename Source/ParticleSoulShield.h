#pragma once
#include "Particle.h"
class ParticleSoulShield :
    public Particle
{

public:

	ParticleSoulShield(iPoint position,GameObject* player, float life = 0, float delay = 0, iPoint velocity = { 0,0 }, std::string name = "Particle");

	~ParticleSoulShield();

	void PreUpdate() override;

	void PostUpdate() override;

private:
	GameObject* player = nullptr;
};

