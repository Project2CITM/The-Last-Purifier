#ifndef _PLAYER_COMBAT_
#define _PLAYER_COMBAT_

#include "Globals.h"
#include "List.h"
#include "SpellList.h"
#include "ExecuteSpell.h"
#include "GameObject.h"
#include "ModuleEvents.h"
#include "Timer.h"

class Player;
class PhysBody;
class PlayerController;
struct b2Vec2;
class DamageArea;
class RevenantWeapon;

class PlayerCombat : GameObject, EventListener
{
public:
	PlayerCombat(std::string name, std::string tag, Player* player);

	void Start() override;

	void PreUpdate() override;

	void Update() override;

	void CombatUpdate();

	void Attack(int chargedTime = 0);

	void CastSpell();

	void ChangeSelectedSpellSlot(int num);

	void StartExecuteSpellCommand(); // 

	/// <summary>
	/// Add the given spellID to either the Spell Slots or the Deck. Returns false if both are full.
	/// </summary>
	/// <param name="spell"></param>
	bool AddSpell(SpellInfo spell);

	bool ChangeWeapon();

	/// <summary>
	/// Checks for empty spell Slots and fills them with any spells in the deck.
	/// </summary>
	void CheckDeck();

	fPoint GetProjectileOffset();

	int GetProjectileRotation();

	b2Vec2 GetAttackOffset();

	void CleanUp();

	int selectedSpell; // The current selected spell slot on the list. This is an index.

	ExecuteSpell* executeSpellCommand = nullptr; // The command that fires the selected spell on the current spell slot to the SpellList.

private:
	/// <summary>
	/// debug function to show current spell and deck slots.
	/// </summary>
	void PrintSlotsState();

	void GameEventTriggered(GameEvent id);

	void SageAttack();

public:

	Player* player = nullptr;

	List<SpellInfo*> spellSlots; // A list of every spell slot and its current spell. If empty, it contains NONE.
	List<SpellInfo*> deckSlots; // A list of every spell in the deck. If empty, contains NONE

private:

	bool isSpellCommandStarted = false;

	float projectileSpeed = 20; //Velocidad de ba

	int chargedAttackTime = 0;

	Timer combatTimer;

	RevenantWeapon* revenantWeapon = nullptr;

	friend class PlayerController;
};

#endif