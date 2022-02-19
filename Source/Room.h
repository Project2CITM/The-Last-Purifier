#ifndef __ROOM_H__
#define __ROOM_H__

#include "GameObject.h"

#define MAX_ROOM_TILES_COLUMNS 25
#define MAX_ROOM_TILES_ROWS 15
//#define MAX_ROOM_ENEMIES 5

struct Door {
	iPoint pos;
	bool open = true;
};

class Room
{
public:
	void CreateDoors();
	void CloseDoors();
	void OpenDoors();

public:
	List<Door*> doors;
	char roomDesign[MAX_ROOM_TILES_ROWS][MAX_ROOM_TILES_COLUMNS];
	iPoint roomPosition; 	//in map, not pixels
};

#endif //_ROOM_H_