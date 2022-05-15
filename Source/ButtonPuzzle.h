#pragma once
#include "GameObject.h"

class ButtonObject;
class Room;

class ButtonPuzzle : public GameObject
{
public:
	ButtonPuzzle(Room* room);

	void Start() override;

	void PreUpdate() override;

	void Update() override;

	void CleanUp();

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
};

