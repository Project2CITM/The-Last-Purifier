#include "ParticleAttackSage.h"
#include "ModuleTextures.h"

ParticleAttackSage::ParticleAttackSage(iPoint position, float life, float delay, iPoint velocity, std::string name) :Particle(position, life, delay, velocity, name)
{

	for (int i = 0; i < 8; i++)
	{
		this->anim.PushBack({ 32 * i,0,32,32 });
	}
	this->anim.loop = true;
	this->anim.hasIdle = false;
	this->anim.speed = 0.5f;

	renderObjects[0].InitAsTexture(app->textures->Load("Assets/Sprites/Player/Sage/basicAttackSageInitial.png"), position, { 0,0,0,0 });
	
}

ParticleAttackSage::~ParticleAttackSage()
{
}
