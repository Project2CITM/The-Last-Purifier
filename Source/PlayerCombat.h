#ifndef _PLAYER_COMBAT_
#define _PLAYER_COMBAT_

#include "Globals.h"
#include "List.h"
#include "SpellList.h"
#include "ExecuteSpell.h"
#include "GameObject.h"
#include "DamageArea.h"

class Player;
class PhysBody;
class PlayerController;
struct b2Vec2;

class PlayerCombat : GameObject
{
public:
	PlayerCombat(std::string name, std::string tag, Player* player);

	void Start() override;

	void Update() override;

	void Attack();

	void CastSpell();

	void ChangeSelectedSpellSlot(int num);

	/// <summary>
	/// Add the given spellID to either the Spell Slots or the Deck. Returns false if both are full.
	/// </summary>
	/// <param name="spell"></param>
	bool AddSpell(SpellInfo spell);

	/// <summary>
	/// Checks for empty spell Slots and fills them with any spells in the deck.
	/// </summary>
	void CheckDeck();

	void CleanUp();

	int availableSpellSlots; // Determines how many available spell slots the player has.
	int availableDeckSlots;

	int selectedSpell; // The current selected spell slot on the list. This is an index.

private:
	/// <summary>
	/// debug function to show current spell and deck slots.
	/// </summary>
	void PrintSlotsState();

	b2Vec2 GetAttackOffset();

	void RevenantAttack();
	void SageAttack();

	// Combat Stats-------------------
	int attackCD;
	int attackCounter;
	bool canAttack;

	int attackAreaCD;
	int attackAreaCounter;
	bool attackAreaActive;

	float projectileSpeed = 10;

	DamageArea* revenantAttack = nullptr;

	List<SpellInfo*> spellSlots; // A list of every spell slot and its current spell. If empty, it contains NONE.
	List<SpellInfo*> deckSlots; // A list of every spell in the deck. If empty, contains NONE
	ExecuteSpell* executeSpellCommand = nullptr; // The command that fires the selected spell on the current spell slot to the SpellList.

	Player* player = nullptr;

	friend class PlayerController;

};

#endif