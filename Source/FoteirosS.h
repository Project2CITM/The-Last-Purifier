#ifndef __FOTEIROS_H__
#define __FOTEIROS_H__

#include "Spell.h"

class FoteirosS : public Spell
{
public:

	FoteirosS();

	void Execute(int level) override;

	void CleanUp() override;

private:

	int damage[3];
};

#endif // !__FOTEIROS_H__