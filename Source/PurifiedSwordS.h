#ifndef _PURIFIED_SWORDS_
#define _PURIFIED_SWORDS_
#include "Spell.h"
#include "ModuleEvents.h"

class PurifiedSwordS : public Spell, public EventListener
{
public:
	PurifiedSwordS();

	~PurifiedSwordS();

	void Execute(int level) override;

	void Update() override;

	void CleanUp() override;

	void GameEventTriggered(GameEvent id) override;


private:
	int damage[3];
	bool stun[3];
	int stunDuration[3];

	int attackUses = 5;
	int currentAttacks = 0;
	int currentLevel = 1;
};
#endif

