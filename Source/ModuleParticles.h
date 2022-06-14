#pragma once

#include "Module.h"
#include "External/SDL_image/include/SDL_image.h"
#include "External/SDL/include/SDL.h"
#include "Point.h"
#include <string>
#include "List.h"


enum EmissorType;
class ParticleEmissor;

#define MAX_NUM_EMITTERS_TYPE 4

class ModuleParticles : public Module
{

private:

	List<ParticleEmissor*> emittersList;
	SDL_Texture* particleAtlas = nullptr;
	std::string nameParticleAtlas = "";

	struct EmitterData
	{
		fPoint angleRange = { 0.0f, 0.0f };
		float maxSpeed = 0.0f;
		float maxSize = 0.0f;
		uint emitNumber = 0u;
		uint emitVariance = 0u;
		uint maxParticleLife = 0u;
		SDL_Rect textureRect = { 0, 0 };
		double lifetime = -1.0f;
		EmissorType type;
		SDL_Color startColor;
		SDL_Color endColor;
		SDL_BlendMode blendMode = SDL_BlendMode::SDL_BLENDMODE_NONE;
	};

	EmitterData vecEmitterData[MAX_NUM_EMITTERS_TYPE];

public:

	ModuleParticles();

	// Destructor
	virtual ~ModuleParticles();

	// Called when before render is available
	bool Init(pugi::xml_node& config);

	// Call before first frame
	bool Start();

	// Called before all Updates
	UpdateStatus PreUpdate();

	// Called each loop iteration
	UpdateStatus Update();

	// Called after all Updates
	UpdateStatus PostUpdate();

	// Called before quitting
	bool CleanUp();

	pugi::xml_node* LoadEmitters(pugi::xml_document& psystem_file) const;

	// Emitter methods
	ParticleEmissor* AddEmiter(fPoint pos, EmissorType type);
	bool RemoveEmitter(ParticleEmissor& emitter);
	bool RemoveAllEmitters();
	SDL_Texture* GetParticleAtlas() const;
	void LoadEmitterData(pugi::xml_node& config, EmissorType type);
};

