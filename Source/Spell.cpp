#include "Spell.h"
#include "Globals.h"
#include "Player.h"
#include "ModuleScene.h"
#include "SceneGame.h"

Spell::Spell()
{
	pugi::xml_parse_result result;
	result = doc.load_file("SpellStats.xml");
	if (result == NULL)
	{
		LOG("Could not load xml file: %s. pugi error: %s", "SpellStats.xml", result.description());
	}
	else
	{
		spellStats = doc.child("stats");
	}

	SceneGame* scene = (SceneGame*)Application::GetInstance()->scene->scenes[Application::GetInstance()->scene->currentScene];
	player = scene->player;
}

Spell::~Spell()
{
}

void Spell::Execute(int level)
{
	printf("Empty Spell Slot!\n");
}

void Spell::Update()
{
}

void Spell::CleanUp()
{
}

int Spell::GetUses()
{
	return uses;
}
