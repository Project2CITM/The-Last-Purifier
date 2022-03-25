#ifndef __ROOM_H__
#define __ROOM_H__

#include "GameObject.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"

#define MAX_ROOM_TILES_COLUMNS 81
#define MAX_ROOM_TILES_ROWS 35
#define TILE_SIZE 16
//#define MAX_ROOM_ENEMIES 5

enum class DoorOrientations {
	RIGHT,
	BOTTOM,
	LEFT,
	TOP
};

struct Door {
	DoorOrientations orientation;
	iPoint pos;
	iPoint size = iPoint(1, 1);
	bool open = true;
	PhysBody* collider = nullptr;
};

class Room
{
public:
	Room()
	{
		this->app = Application::GetInstance();
	}

	void CloseDoors();
	void OpenDoors();

	void DrawRoom();

	void CleanUp();

public:
	Application* app = nullptr;
	List<Door*> doors;
	iPoint roomPosition; 	//in map, not pixels
	SDL_Texture* roomTexture = nullptr;
};

#endif //_ROOM_H_