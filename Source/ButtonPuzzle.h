#ifndef __BUTTON_PUZZLE_H__
#define __BUTTON_PUZZLE_H__

#include "GameObject.h"

class ButtonObject;
class Room;

class ButtonPuzzle : public GameObject
{
public:
	ButtonPuzzle(Room* room);

	void Start() override;

	void PreUpdate() override;

	void CleanUp();

	void SpawnWeaponObject();

private:

	bool CheckResult();

	void ResetPuzzle();
private:

	ButtonObject* buttons[3] = { nullptr };

	iPoint buttonPositions[3] = { {300,300}, {400,400}, {500,500} };

	int buttonOrder[3] = {0,1,2};

	int buttonPressed[3] = {-1,-1,-1};

	int currentButton = 0;

	Room* room = nullptr;

	uint doneSFX = 0;

	uint failSFX = 0;
};

#endif // !__BUTTON_PUZZLE_H__