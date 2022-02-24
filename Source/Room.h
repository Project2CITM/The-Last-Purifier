#ifndef __ROOM_H__
#define __ROOM_H__

#include "GameObject.h"
#include "ModulePhysics.h"

#define MAX_ROOM_TILES_COLUMNS 51
#define MAX_ROOM_TILES_ROWS 31
#define TILE_SIZE 1
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
	bool open = true;
	PhysBody* collider = nullptr;
};

class Room
{
public:
	void CloseDoors(Application* app);
	void OpenDoors(Application* app);

	void CleanUp();

public:
	List<Door*> doors;
	char roomDesign[MAX_ROOM_TILES_ROWS][MAX_ROOM_TILES_COLUMNS];
	iPoint roomPosition; 	//in map, not pixels
};

#endif //_ROOM_H_