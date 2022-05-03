#include "ParticlePurificationSlash.h"

ParticlePurificationSlash::ParticlePurificationSlash(iPoint position, int rot, float life, float delay, bool hasPurifiedSword, iPoint velocity, std::string name) :Particle(position, life, delay, velocity, name)
{
	if (!hasPurifiedSword) InitRenderObjectWithXml("purificationslashrevenant");
	else { InitRenderObjectWithXml("purificationslashwithpurifiedsword"); };

	for (int i = 0; i < 11; i++)
	{
		this->anim.PushBack({ 64 * i,0,64,128 });
	}
	this->anim.loop = false;
	this->anim.hasIdle = false;
	this->anim.duration = 0.008f;
	if (rot == 180)
	{
		renderObjects[0].flip = SDL_FLIP_HORIZONTAL;
	}
	else
	{
		this->rotation = rot;
	}
}


ParticlePurificationSlash::~ParticlePurificationSlash()
{
}
