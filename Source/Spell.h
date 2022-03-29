#ifndef _SPELL_
#define _SPELL_

#include <stdio.h>

enum class SpellID
{
	NONE = 0,
	PURIFIED_SWORD,
	SOUL_SHIELD,
	FLAME_CAPE,
	SACRED_SHIELD,
	COVER,
	PURIFICATION_SLASH,
	PURIFICATION_THRUST,
	DASH_SLASH,
	TORMENT,
	CREMATORIUM
};

class Spell
{
public:
	Spell();

	virtual void Execute(int level);

	virtual void Update();

	virtual void CleanUp();

	int GetUses();

protected:
	int uses = 1;

};
#endif
