#ifndef _PURIFIED_SWORDS_
#define _PURIFIED_SWORDS_
#include "Spell.h"

class PurifiedSwordS : public Spell
{
public:
	PurifiedSwordS();

	void Execute(int level) override;

	void Update() override;

	void CleanUp() override;

private:
	int damage[3];
	bool stun[3];
	int stunDuration[3];
};
#endif

