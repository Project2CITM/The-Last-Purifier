#include "ParticlePneuma.h"
#include "ModuleTextures.h"

ParticlePneuma::ParticlePneuma(iPoint position, int rot, float life, float delay, iPoint velocity, std::string name) : Particle(position, life, delay, velocity, name)
{
	for (int i = 0; i < 89; i++)
	{
		this->anim.PushBack({ 96* i,0,96,96 });
	}
	this->anim.loop = false;
	this->anim.hasIdle = false;
	this->anim.duration = 0.0020f;

	renderObjects[0].InitAsTexture(app->textures->Load("Assets/Sprites/Player/Sage/pneuma_96x96.png"), position, { 0,0,0,0 }, 1, 2, 10);
	renderObjects[0].rotation = rot;
}
