#include "ButtonPuzzle.h"
#include "ButtonObject.h"
#include "Room.h"

ButtonPuzzle::ButtonPuzzle(Room* room) : GameObject("ButtonPuzzle", "ButtonPuzzle")
{
	this->room = room;

	for (int i = 0; i < 3; i++)
	{
		buttonPositions[i] = { (MAX_ROOM_TILES_COLUMNS * TILE_SIZE) * room->roomPosition.x,
							  (MAX_ROOM_TILES_ROWS * TILE_SIZE) * room->roomPosition.y };
	}

	buttonPositions[0] += iPoint(400, 200);

	buttonPositions[1] += iPoint(1200, 400);

	buttonPositions[2] += iPoint(800, 300);

	for (int i = 0; i < 3; i++)
	{
		// Create button
		buttons[i] = new ButtonObject(buttonPositions[i], i);
		
		// Create button order
		int randIndex1 = rand() % 3;
		int randIndex2 = rand() % 3;

		int tempV = buttonOrder[randIndex1];
		buttonOrder[randIndex1] = buttonOrder[randIndex2];
		buttonOrder[randIndex2] = tempV;
	}
}

void ButtonPuzzle::Start()
{
}

void ButtonPuzzle::PreUpdate()
{
	int counter = 0;
	for (int i = 0; i < 3; i++)
	{
		if (buttons[i]->isPressed && !buttons[i]->revised) 
		{ 
			for (int j = 0; j < 3; j++)
			{
				if (buttonPressed[j] == -1) 
				{
					buttonPressed[j] = i; 
					break;
				}
			}
			counter++;
			buttons[i]->revised = true;
		}
	}

	// If every button is pressed
	if (counter != 3) return;

	if (CheckResult())
	{
		room->ClearEnemies();
	}
	else
	{
		ResetPuzzle();
	}

}

void ButtonPuzzle::Update()
{
}

void ButtonPuzzle::CleanUp()
{
}

bool ButtonPuzzle::CheckResult()
{
	int correctButtons = 0;
	for (int i = 0; i < 3; i++)
	{
		if (buttonPressed[i] == buttonOrder[i])correctButtons++;
	}

	if (correctButtons == 3) return true;

	return false;
}

void ButtonPuzzle::ResetPuzzle()
{
	for (int i = 0; i < 3; i++)
	{
		buttons[i]->ResetButton();
		buttonPressed[i] = -1;
	}

}
