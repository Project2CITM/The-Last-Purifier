#include "ModuleParticles.h"
#include "ModuleTextures.h"
#include "ParticleEmissor.h"


ModuleParticles::ModuleParticles() : Module()
{
	name = "psystem";
}

ModuleParticles::~ModuleParticles()
{

}

bool ModuleParticles::Init(pugi::xml_node& config)
{
	bool ret = true;

	pugi::xml_node node;
	node = config.child("emitters");


	nameParticleAtlas = node.child("particleAtlas").attribute("name").as_string();

	for (pugi::xml_node emissors = node.child("particleAtlas").child("emitter"); emissors && ret; emissors = emissors.next_sibling("emitter"))
	{
		std::string emissorType = emissors.attribute("type").as_string();

		if (emissorType == "fire")
			LoadEmitterData(emissors, EmissorType::EMISSOR_TYPE_FIRE);
		else if (emissorType == "blood")
			LoadEmitterData(emissors, EmissorType::EMISSOR_TYPE_BLOOD);
		else if (emissorType == "soul")
			LoadEmitterData(emissors, EmissorType::EMISSOR_TYPE_SOUL);

	}
	return ret;
}

bool ModuleParticles::Start()
{
	particleAtlas = Application::GetInstance()->textures->Load(nameParticleAtlas.c_str());

	return true;
}

UpdateStatus ModuleParticles::PreUpdate()
{
	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleParticles::Update()
{
	//if (emittersList.size() == 0) return UpdateStatus::UPDATE_CONTINUE;
	//std::list<ParticleEmissor*>::const_iterator it;

	ListItem<ParticleEmissor*>* it = emittersList.start;

	while (it != NULL)
	{
		if (it->data != nullptr)
		{
			it->data->Update(app->dt);
		}

		it = it->next;
	}

	/*for (it = emittersList.begin(); it != emittersList.end(); ++it)
	{
		if ((*it) != nullptr)
			(*it)->Update(app->dt);
	}*/

	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleParticles::PostUpdate()
{
	//if (emittersList.size() == 0) return UpdateStatus::UPDATE_CONTINUE;
	//std::list<ParticleEmissor*>::const_iterator it;
	
	//ListItem<ParticleEmissor*>* it = emittersList.start;

	for (int i = 0; i < emittersList.count(); i++)
	{
		if (emittersList[i]->toDestroy)
		{
			emittersList.delPtr(emittersList.At(i));
		}
	}

	//while (it != nullptr)
	//{
	//	if (it->data != nullptr && it->data->toDestroy)
	//	{
	//		//delete (it);
	//		emittersList.delPtr(it);
	//	}
	//	it = it->next;
	//}

	/*for (it = emittersList.begin(); it != emittersList.end(); ++it)
	{
		if ((*it)->toDestroy)
		{
			delete (*it);
			emittersList.erase(it);
		}
		if (emittersList.size() == 0) return UpdateStatus::UPDATE_CONTINUE;
	}*/

	return UpdateStatus::UPDATE_CONTINUE;
}

bool ModuleParticles::CleanUp()
{


	//std::list<ParticleEmissor*>::const_iterator it;
	/*ListItem<ParticleEmissor*>* it = emittersList.start;

	while (it != NULL)
	{
		if (it->data != nullptr)
		{
			delete it;
			emittersList.del(it);
		}
		it = it->next;
	}*/

	/*for (it = emittersList.begin(); it != emittersList.end(); ++it)
	{
		if ((*it) != nullptr)
			delete (*it);
	}*/

	emittersList.clearPtr();
	app->textures->Unload(particleAtlas);

	return true;
}

ParticleEmissor* ModuleParticles::AddEmiter(fPoint pos, EmissorType type)
{
	ParticleEmissor* tmp_emitter = new ParticleEmissor(pos, vecEmitterData[type].emitNumber, vecEmitterData[type].emitVariance, vecEmitterData[type].maxParticleLife, vecEmitterData[type].angleRange, vecEmitterData[type].maxSpeed, vecEmitterData[type].maxSize, vecEmitterData[type].textureRect, vecEmitterData[type].startColor, vecEmitterData[type].endColor, vecEmitterData[type].blendMode, vecEmitterData[type].lifetime);
	//emittersList.push_back(tmp_emitter);
	emittersList.add(tmp_emitter);

	return tmp_emitter;
}

bool ModuleParticles::RemoveEmitter(ParticleEmissor& emitter)
{

	ListItem<ParticleEmissor*>* it = emittersList.start;

	while (it != NULL)
	{
		if (it->data != nullptr && it->data == &emitter)
		{
			it->data->toDestroy = true;
			return true;
		}
		it = it->next;
	}

	/*std::list<ParticleEmissor*>::const_iterator it;

	for (it = emittersList.begin(); it != emittersList.end(); ++it)
	{

		if ((*it) == &emitter)
		{
			(*it)->toDestroy = true;
			return true;
		}
	}*/

	return false;
}

bool ModuleParticles::RemoveAllEmitters()
{
	bool ret = false;

	ListItem<ParticleEmissor*>* it = emittersList.start;

	while (it != NULL)
	{
		if (it->data != nullptr)
		{
			it->data->toDestroy;
			ret = true;
		}
		it = it->next;
	}

	/*std::list<ParticleEmissor*>::const_iterator it;

	for (it = emittersList.begin(); it != emittersList.end(); ++it)
	{
		if ((*it) != nullptr) (*it)->toDestroy = true;
		ret = true;
	}*/

	return ret;
}

SDL_Texture* ModuleParticles::GetParticleAtlas() const
{
	return particleAtlas;
}

void ModuleParticles::LoadEmitterData(pugi::xml_node& emitter, EmissorType type)
{
	EmitterData tmp;

	// Angle range
	tmp.angleRange.x = emitter.child("angleRange").attribute("min").as_float();
	tmp.angleRange.y = emitter.child("angleRange").attribute("max").as_float();

	tmp.maxSpeed = emitter.child("maxSpeed").attribute("value").as_float();
	tmp.maxSize = emitter.child("maxSize").attribute("value").as_float();
	tmp.emitNumber = emitter.child("emitNumber").attribute("value").as_uint();
	tmp.emitVariance = emitter.child("emitVariance").attribute("value").as_uint();
	tmp.maxParticleLife = emitter.child("maxParticleLife").attribute("value").as_uint();

	// Rect from particle atlas
	tmp.textureRect.x = emitter.child("textureRect").attribute("x").as_int();
	tmp.textureRect.y = emitter.child("textureRect").attribute("y").as_int();
	tmp.textureRect.w = emitter.child("textureRect").attribute("w").as_int();
	tmp.textureRect.h = emitter.child("textureRect").attribute("h").as_int();

	// Lifetime of emitter
	tmp.lifetime = emitter.child("lifetime").attribute("value").as_double();

	// Start color
	tmp.startColor.r = emitter.child("startColor").attribute("r").as_uint();
	tmp.startColor.g = emitter.child("startColor").attribute("g").as_uint();
	tmp.startColor.b = emitter.child("startColor").attribute("b").as_uint();
	tmp.startColor.a = emitter.child("startColor").attribute("a").as_uint();

	// End color
	tmp.endColor.r = emitter.child("endColor").attribute("r").as_uint();
	tmp.endColor.g = emitter.child("endColor").attribute("g").as_uint();
	tmp.endColor.b = emitter.child("endColor").attribute("b").as_uint();
	tmp.endColor.a = emitter.child("endColor").attribute("a").as_uint();

	// Blend mode
	std::string blendModeString = emitter.child("blendMode").attribute("mode").as_string();

	if (blendModeString == "add")
		tmp.blendMode = SDL_BlendMode::SDL_BLENDMODE_ADD;
	else if (blendModeString == "blend")
		tmp.blendMode = SDL_BlendMode::SDL_BLENDMODE_BLEND;
	else if (blendModeString == "mod")
		tmp.blendMode = SDL_BlendMode::SDL_BLENDMODE_MOD;
	else if (blendModeString == "none")
		tmp.blendMode = SDL_BlendMode::SDL_BLENDMODE_NONE;

	vecEmitterData[type] = tmp;
}

pugi::xml_node* ModuleParticles::LoadEmitters(pugi::xml_document& psystem_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = psystem_file.load_file("psystem_config.xml");

	if (result == NULL)
	{
		LOG("Could not load xml file config.xml. pugi error: %s", result.description());
	}
	else
	{
		ret = psystem_file.child("emitters");
	}
		
	return &ret;
}