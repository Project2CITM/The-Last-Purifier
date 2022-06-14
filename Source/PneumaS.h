#ifndef __PNEUMAS_H__
#define __PNEUMAS_H__

#include "Spell.h"
#include "Globals.h"

class PneumaS : public Spell
{
public:
	PneumaS();

	void Execute(int level) override;

private:

	int damage[3];
	uint executeSFX;
};

#endif // !__PNEUMAS_H__