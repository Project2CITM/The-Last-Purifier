#ifndef __PURIFICATION_SLASH_H__
#define __PURIFICATION_SLASH_H__

#include "Spell.h"
#include "Point.h"
#include "External/Box2D/include/Box2D.h"

class DamageArea;

class PurificationSlashS : public Spell
{
public:
	PurificationSlashS();

	void Execute(int level) override;

	void Update() override;

	void CleanUp() override;

private:

	b2Vec2 GetPlayerOffset();

	DamageArea* damageArea = nullptr;

	int attackFrames = 5;

	int currentFrame = 0;

	int damage[3];

	uint slashSFX = 0;
};

#endif // !__PURIFICATION_SLASH_H__