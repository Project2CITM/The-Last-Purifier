#include "ParticleAttackKaboom.h"
#include "ModuleAudio.h"

ParticleAttackKaboom::ParticleAttackKaboom(iPoint position) :Particle(position, 1, 0)
{
	InitRenderObjectWithXml("kaboomExplotion");

	for (int i = 0; i < 7; i++)
	{
		anim.PushBack({ i * 32,0,32,32 });
	}

	anim.loop = false;

	anim.duration = 0.106f;

	anim.hasIdle = false;
}

ParticleAttackKaboom::~ParticleAttackKaboom()
{
}

void ParticleAttackKaboom::PreUpdate()
{
	if (anim.HasFinished()) pendingToDelete = true;
}

void ParticleAttackKaboom::PostUpdate()
{
	Particle::PostUpdate();
}
