#pragma once
#include "Particle.h"
class PaticleHitFotieros :
    public Particle
{

public:

	PaticleHitFotieros(iPoint position, float life = 0, float delay = 0, iPoint velocity = { 0,0 }, std::string name = "Particle");

	~PaticleHitFotieros();


};

