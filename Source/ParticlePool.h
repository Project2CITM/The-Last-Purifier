#pragma once

#include "SingleParticle.h"
#include "ParticleEmissor.h"
#include "Point.h"
#include <vector>

class ParticleEmissor;

class ParticlePool
{
private:

	int poolSize = 0;
	SingleParticle* firstAvailable;
	std::vector<SingleParticle*> particlesVec;
	SingleParticle* particleArray = nullptr;

public:

	ParticlePool(ParticleEmissor* emitter);
	virtual ~ParticlePool();

	void Generate(fPoint pos, float speed, float angle, float start_radius, uint life, SDL_Rect textureRect, SDL_Color startColor, SDL_Color endColor, SDL_BlendMode blendMode);
	bool Update(float dt);
};

