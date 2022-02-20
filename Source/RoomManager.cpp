#include "RoomManager.h"

void RoomManager::Start()
{
	GenerateMap(20);
	//incloure textures
	CreateDoors();
}

void RoomManager::Update()
{
}

void RoomManager::PostUpdate()
{
	app->renderer->AddRectRenderQueue(SDL_Rect{ 0, 0, MAX_ROOMS_COLUMNS * MAX_ROOM_TILES_COLUMNS * ROOM_SCALE,
													  MAX_ROOMS_ROWS * MAX_ROOM_TILES_ROWS * ROOM_SCALE }, SDL_Color{ 0, 170, 230, 255});
	DrawRooms();
}

void RoomManager::CleanUp()
{
	rooms.clearPtr();
}

void RoomManager::GenerateMap(short RoomNumber)
{
	if (RoomNumber < 1 || RoomNumber > MAX_ROOMS_ROWS * MAX_ROOMS_COLUMNS)
		return;

	srand(time(NULL));
	iPoint p;
	p.x = rand() % MAX_ROOMS_ROWS;
	p.y = rand() % MAX_ROOMS_COLUMNS;
	Room* r = CreateRoom(p);
	while (RoomNumber > 1) {
		iPoint pos = r->roomPosition;
		if (CheckAdjacentSpace(r)) {	//there is space next to the current room to spawn another
			short random = rand() % 4;
			switch (random) {
			case 0:
				pos.x++;	break;
			case 1:
				pos.y++;	break;
			case 2:
				pos.x--;	break;
			case 3:
				pos.y--;	break;
			}
			if ((pos.x < MAX_ROOMS_ROWS && pos.x >= 0 && pos.y < MAX_ROOMS_COLUMNS && pos.y >= 0) && roomPositions[pos.x][pos.y] == nullptr) {
				r = CreateRoom(iPoint(pos.x, pos.y));
				--RoomNumber;
			}
		}
		else {	//room is trapped, chose another one
			r = rooms.At(rand() % rooms.count())->data;
		}
		
	}
}

bool RoomManager::CheckAdjacentSpace(Room* r)
{
	int x = r->roomPosition.x;
	int y = r->roomPosition.y;

	return (roomPositions[x + 1][y] == nullptr && x+1 >= 0 && x+1 < MAX_ROOMS_COLUMNS) ||
		(roomPositions[x][y + 1] == nullptr && y + 1 >= 0 && y + 1 < MAX_ROOMS_ROWS) ||
		(roomPositions[x - 1][y] == nullptr && x - 1 >= 0 && x - 1 < MAX_ROOMS_COLUMNS) ||
		(roomPositions[x][y - 1] == nullptr && y - 1 >= 0 && y - 1 < MAX_ROOMS_ROWS);
}

void RoomManager::CreateDoors()
{
	ListItem<Room*>* currentRoom = rooms.start;
	while (currentRoom != nullptr) {
		currentRoom->data->CreateDoors();
		currentRoom = currentRoom->next;
	}
}

Room* RoomManager::CreateRoom(iPoint mapPosition)
{
	Room* r = new Room();
	r->roomPosition = mapPosition;
	//r->roomDesign = 
	r->CreateDoors();
	rooms.add(r);
	roomPositions[mapPosition.x][mapPosition.y] = r;
	return r;
}

void RoomManager::DrawRooms()
{
	uint rn = rooms.count();
	SDL_Color c = SDL_Color{ 255, 255, 255, 255 };
	//SDL_Color c = SDL_Color{ 255, 190, 200, 255 };

	ListItem<Room*>* currentRoom = rooms.start;
	while (currentRoom != nullptr) {
		app->renderer->AddRectRenderQueue(SDL_Rect{ currentRoom->data->roomPosition.x * MAX_ROOM_TILES_COLUMNS * ROOM_SCALE, 
													currentRoom->data->roomPosition.y * MAX_ROOM_TILES_ROWS* ROOM_SCALE,
													MAX_ROOM_TILES_COLUMNS* ROOM_SCALE, MAX_ROOM_TILES_ROWS* ROOM_SCALE }, 
													c);
		c.r -= 255/rn;
		c.g -= 255/rn;
		c.b -= 255/rn;
		currentRoom = currentRoom->next;
	}
}
