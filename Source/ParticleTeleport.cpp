#include "ParticleTeleport.h"
#include "ModuleTextures.h"

ParticleTeleport::ParticleTeleport(iPoint position) : Particle(position, 0.8f, 0, { 0,0 }, "ParticleTeleport")
{
	this->position.x -= 40;
	this->position.y -= 40;


	for (int i = 0; i < 73; i++)
	{
		this->anim.PushBack({ 100 * i,0,100,100 });
	}
	this->anim.loop = false;
	this->anim.hasIdle = false;
	this->anim.duration = 0.0020f;

	renderObjects[0].InitAsTexture(app->textures->Load("Assets/Sprites/Player/teleportParticle100x100_73frames.png"), position, { 0,0,0,0 }, 0.8f);
	renderObjects[0].layer = 3;
	renderObjects[0].color.a = 155;
}
