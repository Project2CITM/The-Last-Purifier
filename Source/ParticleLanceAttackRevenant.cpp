#include "ParticleLanceAttackRevenant.h"

ParticleLanceAttackRevenant::ParticleLanceAttackRevenant(iPoint position, int rot, float life, float delay, bool hasPurifiedSword, SDL_RendererFlip flip, iPoint velocity, std::string name) :Particle(position, life, delay, velocity, name)
{
	if (!hasPurifiedSword)InitRenderObjectWithXml("lanceattackrevenant");
	else { InitRenderObjectWithXml("lanceattackrevenantwithpurifiedsword"); }

	for (int i = 0; i < 6; i++)
	{
		this->anim.PushBack({ 0,60*i,256,60 });
	}
	this->anim.loop = false;
	this->anim.hasIdle = false;
	this->anim.duration = 0.008f;

	this->rotation = rot;

	renderObjects[0].flip = flip;
	renderObjects[0].rotation = rot;

	if (flip != SDL_FLIP_NONE) renderObjects[0].flip = flip;
}

ParticleLanceAttackRevenant::~ParticleLanceAttackRevenant()
{
}
