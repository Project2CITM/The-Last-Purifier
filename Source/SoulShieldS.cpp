#include "SoulShieldS.h"
#include "Player.h"
#include "ParticleSoulShield.h"
#include "PlayerController.h"
#include "Application.h"
#include "ModuleAudio.h"

SoulShieldS::SoulShieldS()
{
	spellStats = spellStats.child("soul_shield");

	for (int i = 0; i < 3; i++)
	{
		std::string lvl = "lvl" + std::to_string(i + 1);

		shield[i] = spellStats.child("shield").attribute(lvl.c_str()).as_int();
		frames[i] = spellStats.child("duration").attribute(lvl.c_str()).as_int();
	}

	uses = spellStats.child("uses").attribute("quantity").as_int();

	executeSFX = Application::GetInstance()->audio->LoadFx("Assets/Audio/SFX/Spells/sfx_soulShield.wav", false);

	spellName = spellStats.child("name").attribute("name").as_string();
}

void SoulShieldS::Execute(int level)
{
	if (currentDuration > 0) player->ChangeShield(-shield[currentLevel]);

	currentLevel = level-1;
	currentDuration = frames[currentLevel];

	player->ChangeShield(shield[currentLevel]);

	float particlelifetimeinseconds = (float)(frames[currentLevel]) / 60.0f;
	
	isOn = true;

	new ParticleSoulShield(player->controller->GetPosition(), player->controller, particlelifetimeinseconds);

	Application::GetInstance()->audio->PlayFx(executeSFX);
}

void SoulShieldS::Update()
{
	if (!isOn) return;
	currentDuration--;

	if (currentDuration <= 0)
	{
		player->ChangeShield(-shield[currentLevel]);
		isOn = false;
	}
}

void SoulShieldS::CleanUp()
{
}