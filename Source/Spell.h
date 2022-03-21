#ifndef _SPELL_
#define _SPELL_

#include <stdio.h>

class Spell
{
public:
	Spell();

	virtual bool Execute();

	virtual void Update();

	virtual void CleanUp();

};
#endif
