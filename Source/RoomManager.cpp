#include "RoomManager.h"

void RoomManager::Start()
{
	GenerateMap(1);
	//incloure textures
	CreateDoors();
}

void RoomManager::Update()
{
}

void RoomManager::PostUpdate()
{
	app->renderer->AddRectRenderQueue(SDL_Rect{ 0, 0, MAX_ROOMS_COLUMNS * MAX_ROOM_TILES_COLUMNS * ROOM_SCALE, 
													  MAX_ROOMS_ROWS * MAX_ROOM_TILES_ROWS * ROOM_SCALE}, 0, 170, 230);
	DrawRooms();
}

void RoomManager::CleanUp()
{
	rooms.clearPtr();
}

void RoomManager::GenerateMap(short RoomNumber)
{
	CreateRoom(iPoint(1, 1));
	CreateRoom(iPoint(3, 4));
}

void RoomManager::CreateDoors()
{
	ListItem<Room*>* currentRoom = rooms.start;
	while (currentRoom != nullptr) {
		currentRoom->data->CreateDoors();
		currentRoom = currentRoom->next;
	}
}

void RoomManager::CreateRoom(iPoint mapPosition)
{
	Room* r = new Room();
	r->roomPosition = mapPosition;
	//r->roomDesign = 
	r->CreateDoors();
	rooms.add(r);
	roomPositions[mapPosition.x][mapPosition.y] = r;
}

void RoomManager::DrawRooms()
{
	ListItem<Room*>* currentRoom = rooms.start;
	while (currentRoom != nullptr) {
		app->renderer->AddRectRenderQueue(SDL_Rect{ currentRoom->data->roomPosition.x * MAX_ROOM_TILES_COLUMNS * ROOM_SCALE, 
													currentRoom->data->roomPosition.y * MAX_ROOM_TILES_ROWS* ROOM_SCALE,
													MAX_ROOM_TILES_COLUMNS * ROOM_SCALE, MAX_ROOM_TILES_ROWS * ROOM_SCALE }, 255, 192, 203);
		currentRoom = currentRoom->next;
	}
}
