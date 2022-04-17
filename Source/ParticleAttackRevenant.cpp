#include "ParticleAttackRevenant.h"

ParticleAttackRevenant::ParticleAttackRevenant(iPoint position, int rot, float life, float delay, bool hasPurifiedSword, iPoint velocity, std::string name):Particle(position,life,delay,velocity,name)
{
	if (!hasPurifiedSword) InitRenderObjectWithXml("basicwithpurifiedsword");//InitRenderObjectWithXml("basicattackrevenant");
	else { InitRenderObjectWithXml("basicwithpurifiedsword"); };

	for (int i = 0; i < 11; i++)
	{
		this->anim.PushBack({ 64 * i,0,64,64 });
	}
	this->anim.loop = false;
	this->anim.hasIdle = false;
	this->anim.speed = 2.0f;
	if (rot == 180)
	{
		renderObjects[0].flip = SDL_FLIP_HORIZONTAL;
	}
	else
	{
		this->rotation = rot;
	}
}

ParticleAttackRevenant::~ParticleAttackRevenant()
{
}



