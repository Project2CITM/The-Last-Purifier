#include "ParticleAttackRevenant.h"

ParticleAttackRevenant::ParticleAttackRevenant(iPoint position, int rot, float life, float delay, bool hasPurifiedSword, SDL_RendererFlip flip, iPoint velocity, std::string name):Particle(position,life,delay,velocity,name)
{
	if (!hasPurifiedSword) InitRenderObjectWithXml("basicattackrevenant");
	else { InitRenderObjectWithXml("basicwithpurifiedsword"); };

	for (int i = 0; i < 11; i++)
	{
		this->anim.PushBack({ 64 * i,0,64,64 });
	}
	this->anim.loop = false;
	this->anim.hasIdle = false;
	this->anim.duration = 0.008f;

	this->rotation = rot;

	renderObjects[0].flip = flip;

	if (flip != SDL_FLIP_NONE) renderObjects[0].flip = flip;
	
}

ParticleAttackRevenant::~ParticleAttackRevenant()
{
}



