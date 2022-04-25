#pragma once
#include "Spell.h"
#include "ModuleEvents.h"

class TestSpell : public Spell, public EventListener
{
public:
	TestSpell();

	void Execute(int level) override;
	
	void Update() override;

	void GameEventTriggered(GameEvent id) override;

};

