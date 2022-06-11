#pragma once

#include "External/SDL/include/SDL.h"
#include "Point.h"
#include "PerfTimer.h"
#include "ModuleParticles.h"

class ParticlePool;

enum EmissorType
{
	EMISSOR_TYPE_NONE = -1,
	EMISSOR_TYPE_FIRE,
	EMISSOR_TYPE_BLOOD
};

class ParticleEmissor
{
private:

	// Particles size and movement
	fPoint pos = { 0.0f, 0.0f };
	fPoint angleRange = { 0.0f, 360.0f };
	float maxSpeed = 0.0f;
	float maxSize = 0.0f;

	// Particles emission
	int emissionRate = 0;
	uint emitNumber = 0u;
	uint emitVariance = 0u;
	uint maxParticleLife = 0u;

	uint maxParticlesPerFrame = 0u;

	// Pool
	ParticlePool* emitterPool = nullptr;
	int poolSize = 0;

	bool active = false;
	SDL_Rect textureRect;
	PerfTimer stopTimer;
	PerfTimer emissionTimer;
	PerfTimer lifeTimer;
	double stopTime = 0.0f;
	double emissionTime = 0.0f;
	double lifetime = -1.0f;
	EmissorType type = EmissorType::EMISSOR_TYPE_NONE;

	SDL_Color startColor = { 0, 0, 0, 0 };
	SDL_Color endColor = { 0, 0, 0, 0 };
	SDL_BlendMode blendMode = SDL_BlendMode::SDL_BLENDMODE_NONE;
	float timeStep = 0.0f;

public:

	bool toDestroy = false;


	ParticleEmissor(fPoint pos, uint emitNumber, uint emitVariance, uint maxParticleLife, fPoint angleRange, float maxSpeed, float maxSize, SDL_Rect textureRect, SDL_Color startColor = { 0, 0, 0, 0 }, SDL_Color endColor = { 0, 0, 0, 0 }, SDL_BlendMode blendMode = SDL_BlendMode::SDL_BLENDMODE_NONE, double lifetime = -1.0f);
	virtual ~ParticleEmissor();

	void Update(float dt);
	float RangeRandomNum(float min = -1.0f, float max = 1.0f);
	int GetPoolSize() const;
	void StartEmission(double timer = -1.0f);
	void StopEmission(double timer = 0.0f);
	void MoveEmitter(fPoint newPos);
	fPoint GetEmitterPos() const;
};

