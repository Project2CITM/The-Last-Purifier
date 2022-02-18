#ifndef __ROOMMANAGER_H__
#define __ROOMMANAGER_H__

#include "Room.h"
#include "GameObject.h"

#define MAX_ROOMS_COLUMNS 8
#define MAX_ROOMS_ROWS 8

class RoomManager : public GameObject{
	
public:
	void GenerateMap(short RoomNumber);

public:
	List<Room*> rooms;
	SDL_Texture* tile_texture = nullptr;
};

#endif //__ROOMMANAGER_H_