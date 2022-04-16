#include "ParticleAttackRevenant.h"

ParticleAttackRevenant::ParticleAttackRevenant(iPoint position, float life, float delay, iPoint velocity, std::string name):Particle(position,life,delay,velocity,name)
{
	InitRenderObjectWithXml("basicattackrevenant");

	for (int i = 0; i < 11; i++)
	{
		this->anim.PushBack({ 64 * i,0,64,64 });
	}
	this->anim.loop = false;
	this->anim.hasIdle = false;
	this->anim.speed = 0.5f;
}

ParticleAttackRevenant::~ParticleAttackRevenant()
{
}


