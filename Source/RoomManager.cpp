#include "RoomManager.h"

void RoomManager::Start()
{
	GenerateMap(25);
	//incloure textures
	CreateDoors();
	rooms[10]->CloseDoors(app);
}

void RoomManager::Update()
{
}

void RoomManager::PostUpdate()
{
	app->renderer->AddRectRenderQueue(SDL_Rect{ 0, 0, MAX_ROOMS_COLUMNS * MAX_ROOM_TILES_COLUMNS * TILE_SIZE,
													  MAX_ROOMS_ROWS * MAX_ROOM_TILES_ROWS * TILE_SIZE }, SDL_Color{ 0, 170, 230, 255});
	DrawRooms();
	DrawDoors();

}

void RoomManager::CleanUp()
{
	ListItem<Room*>* currentRoom = rooms.start;
	while (currentRoom != nullptr) {
		currentRoom->data->CleanUp();
		currentRoom = currentRoom->next;
	}

	rooms.clearPtr();
}

//FUNCTIONS
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
		Room* cr = currentRoom->data;

		//Right Door
		if (roomPositions[cr->roomPosition.x + 1][cr->roomPosition.y] != nullptr && cr->roomPosition.x + 1 < MAX_ROOMS_COLUMNS) {
			Door* d = new Door();
			d->orientation = DoorOrientations::RIGHT;
			d->pos.x = (cr->roomPosition.x + 1) * MAX_ROOM_TILES_COLUMNS * TILE_SIZE - TILE_SIZE;
			d->pos.y = cr->roomPosition.y * MAX_ROOM_TILES_ROWS * TILE_SIZE + (MAX_ROOM_TILES_ROWS * TILE_SIZE / 2);
			cr->doors.add(d);
		}

		//Bottom Door
		if (roomPositions[cr->roomPosition.x][cr->roomPosition.y + 1] != nullptr && cr->roomPosition.y + 1 < MAX_ROOMS_ROWS) {
			Door* d = new Door();
			d->orientation = DoorOrientations::BOTTOM;
			d->pos.x = cr->roomPosition.x * MAX_ROOM_TILES_COLUMNS * TILE_SIZE + (MAX_ROOM_TILES_COLUMNS * TILE_SIZE / 2);
			d->pos.y = (cr->roomPosition.y + 1) * MAX_ROOM_TILES_ROWS * TILE_SIZE - TILE_SIZE;
			cr->doors.add(d);
		}

		//Left Door
		if (roomPositions[cr->roomPosition.x - 1][cr->roomPosition.y] != nullptr && cr->roomPosition.x - 1 >= 0) {
			Door* d = new Door();
			d->orientation = DoorOrientations::LEFT;
			d->pos.x = cr->roomPosition.x * MAX_ROOM_TILES_COLUMNS * TILE_SIZE;
			d->pos.y = cr->roomPosition.y * MAX_ROOM_TILES_ROWS * TILE_SIZE + (MAX_ROOM_TILES_ROWS * TILE_SIZE / 2);
			cr->doors.add(d);
		}

		//Top Door
		if (roomPositions[cr->roomPosition.x][cr->roomPosition.y - 1] != nullptr && cr->roomPosition.y - 1 >= 0) {
			Door* d = new Door();
			d->orientation = DoorOrientations::TOP;
			d->pos.x = cr->roomPosition.x * MAX_ROOM_TILES_COLUMNS * TILE_SIZE + (MAX_ROOM_TILES_COLUMNS * TILE_SIZE / 2);
			d->pos.y = cr->roomPosition.y * MAX_ROOM_TILES_ROWS * TILE_SIZE;
			cr->doors.add(d);
		}

		currentRoom = currentRoom->next;
	}
}

Room* RoomManager::CreateRoom(iPoint mapPosition)
{
	Room* r = new Room();
	r->roomPosition = mapPosition;
	//r->roomDesign = 
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
		app->renderer->AddRectRenderQueue(SDL_Rect{ currentRoom->data->roomPosition.x * MAX_ROOM_TILES_COLUMNS * TILE_SIZE,
													currentRoom->data->roomPosition.y * MAX_ROOM_TILES_ROWS* TILE_SIZE,
													MAX_ROOM_TILES_COLUMNS* TILE_SIZE, MAX_ROOM_TILES_ROWS* TILE_SIZE },
													c);
		c.r -= 255/rn;
		c.g -= 255/rn;
		c.b -= 255/rn;
		currentRoom = currentRoom->next;
	}
}

void RoomManager::DrawDoors()
{
	ListItem<Room*>* currentRoom = rooms.start;
	while (currentRoom != nullptr) {
		ListItem<Door*>* currentDoor = currentRoom->data->doors.start;
		while (currentDoor != nullptr) {
			app->renderer->AddRectRenderQueue(SDL_Rect{ currentDoor->data->pos.x, currentDoor->data->pos.y, TILE_SIZE, TILE_SIZE },
				SDL_Color{ 255, 0, 255, 255 }, 1, 0.0f, false);
			
			if (currentDoor->data->collider != nullptr) {
				iPoint npos;
				currentDoor->data->collider->GetCenterPosition(npos.x, npos.y);
				app->renderer->AddRectRenderQueue(SDL_Rect{ npos.x, npos.y, currentDoor->data->collider->width, currentDoor->data->collider->height },
					SDL_Color{ 255, 255, 255, 255 }, 1, 0.0f, false);
			}

			currentDoor = currentDoor->next;
		}
		currentRoom = currentRoom->next;
	}
}
