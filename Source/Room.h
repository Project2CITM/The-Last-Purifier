#ifndef __ROOM_H__
#define __ROOM_H__

#include "GameObject.h"

#define MAX_ROOM_TILES_COLUMNS 25
#define MAX_ROOM_TILES_ROWS 15
//#define MAX_ROOM_ENEMIES 5

class Room
{
	char roomDesign[MAX_ROOM_TILES_ROWS][MAX_ROOM_TILES_COLUMNS];
	iPoint roomPosition; 	//in map, not pixels
	//iPoint roomEnemyPositions[MAX_ROOM_ENEMIES];

	void DrawRoom();
	//void SpawnRoomEnemies();
};

#endif //_ROOM_H_