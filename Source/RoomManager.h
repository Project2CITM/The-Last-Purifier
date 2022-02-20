#ifndef __ROOMMANAGER_H__
#define __ROOMMANAGER_H__

#include "Room.h"
#include "GameObject.h"
#include "Timer.h"

#define MAX_ROOMS_COLUMNS 8
#define MAX_ROOMS_ROWS 8
#define ROOM_SCALE 3

class RoomManager{
	
public:
	RoomManager(Application* app) {
		this->app = app;
	}

	void Start();
	void Update();
	void PostUpdate();
	void CleanUp();

	void GenerateMap(short RoomNumber);
	bool CheckAdjacentSpace(Room* r);
	void CreateDoors();
	Room* CreateRoom(iPoint mapPosition);

	void DrawRooms();

public:
	Application* app = nullptr;
	List<Room*> rooms;
	Room* roomPositions[MAX_ROOMS_COLUMNS][MAX_ROOMS_ROWS];
	SDL_Texture* tile_texture = nullptr;
};

#endif //__ROOMMANAGER_H_