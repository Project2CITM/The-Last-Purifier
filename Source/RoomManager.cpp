#include "RoomManager.h"
#include "ModuleRender.h"

void RoomManager::Start()
{
	//incloure textures

	GenerateMap(25);

	CreateDoors();
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
	for (int i = 0; i < MAX_ROOMS_COLUMNS; ++i) {
		for (int j = 0; j < MAX_ROOMS_ROWS; j++) {
			roomPositions[i][j] = nullptr;
		}
	}
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
	//if the Room number is too big or too little
	if (RoomNumber < 1 || RoomNumber > MAX_ROOMS_ROWS * MAX_ROOMS_COLUMNS)
		return;

	//srand(time(NULL));
	//Generate first room
	iPoint p;
	/*p.x = rand() % MAX_ROOMS_COLUMNS;
	p.y = rand() % MAX_ROOMS_ROWS;*/
	p.x = MAX_ROOMS_COLUMNS / 2;
	p.y = MAX_ROOMS_ROWS / 2;
	Room* r = CreateRoom(p);

	//Create all rooms except the first and Boss room
	while (RoomNumber > 2) {
		iPoint pos = r->roomPosition;
		if (CheckAdjacentSpace(r) > 0) {	//there is space next to the current room to spawn another
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
			//if the room is not out of boundaries and not occupied, create one
			if ((pos.x < MAX_ROOMS_COLUMNS && pos.x >= 0 && pos.y < MAX_ROOMS_ROWS && pos.y >= 0) && roomPositions[pos.x][pos.y] == nullptr) {
				r = CreateRoom(iPoint(pos.x, pos.y));
				--RoomNumber;
			}
		}
		else {	//room is trapped, chose another one
			r = rooms.At(rand() % rooms.count())->data;
		}
	}

	//BOSS ROOM
	iPoint bossRoomPos = iPoint(-1, -1);	//initialize the Boss room position
	iPoint startRoomPos = rooms[0]->roomPosition;
	int adjacentSpaces = 3;	//Blank spaces that the Boss room should have

	do {	//check all rooms that have 3 spaces left
		for (int i = 0; i < MAX_ROOMS_COLUMNS; ++i) {
			for (int j = 0; j < MAX_ROOMS_ROWS; ++j) {
				//if the boss room is not initialized, do it
				if (bossRoomPos != iPoint(-1, -1)) {
					//if the room is nearer to the start than the last one, don't check
					if (((bossRoomPos.x - startRoomPos.x) * (bossRoomPos.x - startRoomPos.x) + (bossRoomPos.y - startRoomPos.y) * (bossRoomPos.y - startRoomPos.y))
						< ((i - startRoomPos.x) * (i - startRoomPos.x) + (j - startRoomPos.y) * (j - startRoomPos.y))) {
						//check the space is not ocupied, and the number of spaces adjacent to it is correct
						if (CheckAdjacentSpace(iPoint(i, j)) == adjacentSpaces && roomPositions[i][j] == nullptr) {
							bossRoomPos = iPoint(i, j);
						}
					}
				}
				else {
					if (CheckAdjacentSpace(iPoint(i, j)) == adjacentSpaces && roomPositions[i][j] == nullptr) {
						bossRoomPos = iPoint(i, j);
					}
				}
			}
		}
		adjacentSpaces--;
	} while (bossRoomPos == iPoint(-1,-1));

	bossRoom = bossRoomPos;
	CreateRoom(bossRoomPos);
}

//Check the number of blank spaces next to the room
int RoomManager::CheckAdjacentSpace(Room* r)
{
	int x = r->roomPosition.x;
	int y = r->roomPosition.y;
	int freespaces = 0;

	if (roomPositions[x + 1][y] == nullptr && x + 1 >= 0 && x + 1 < MAX_ROOMS_COLUMNS)
		freespaces++;

	if (roomPositions[x][y + 1] == nullptr && y + 1 >= 0 && y + 1 < MAX_ROOMS_ROWS)
		freespaces++;

	if (roomPositions[x - 1][y] == nullptr && x - 1 >= 0 && x - 1 < MAX_ROOMS_COLUMNS)
		freespaces++;

	if (roomPositions[x][y - 1] == nullptr && y - 1 >= 0 && y - 1 < MAX_ROOMS_ROWS)
		freespaces++;

	return freespaces;
}

//Check the number of blank spaces next to the room
int RoomManager::CheckAdjacentSpace(iPoint p)
{
	int x = p.x;
	int y = p.y;
	int spaces = 0;

	if (x + 1 >= 0 && x + 1 < MAX_ROOMS_COLUMNS) {
		if (roomPositions[x + 1][y] == nullptr)
			spaces++;
	}
	else {
		spaces++;
	}

	if (y + 1 >= 0 && y + 1 < MAX_ROOMS_ROWS) {
		if (roomPositions[x][y + 1] == nullptr)
			spaces++;
	}
	else {
		spaces++;
	}

	if (x - 1 >= 0 && x - 1 < MAX_ROOMS_COLUMNS) {
		if (roomPositions[x - 1][y] == nullptr)
			spaces++;
	}
	else {
		spaces++;
	}

	if (y - 1 >= 0 && y - 1 < MAX_ROOMS_ROWS) {
		if (roomPositions[x][y - 1] == nullptr)
			spaces++;
	}
	else {
		spaces++;
	}

	return spaces;
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

//Pseudo-print the rooms
void RoomManager::DrawRooms()
{
	uint rn = rooms.count();
	SDL_Color c = SDL_Color{ 255, 255, 255, 255 };

	ListItem<Room*>* currentRoom = rooms.start;
	while (currentRoom != nullptr) {
		//change boss room color
		if (currentRoom->data->roomPosition == bossRoom)
			c = SDL_Color{ 255, 10, 10, 255 };

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

//Pseudo-print the doors
void RoomManager::DrawDoors()
{
	ListItem<Room*>* currentRoom = rooms.start;
	while (currentRoom != nullptr) {
		ListItem<Door*>* currentDoor = currentRoom->data->doors.start;
		while (currentDoor != nullptr) {
			app->renderer->AddRectRenderQueue(SDL_Rect{ currentDoor->data->pos.x, currentDoor->data->pos.y, TILE_SIZE, TILE_SIZE },
				SDL_Color{ 255, 100, 255, 255 }, false, 1, 0.0f);

			//if (currentDoor->data->collider != nullptr) {
			//	iPoint npos;
			//	currentDoor->data->collider->GetCenterPosition(npos.x, npos.y);
			//	app->renderer->AddRectRenderQueue(SDL_Rect{ npos.x, npos.y, currentDoor->data->collider->width, currentDoor->data->collider->height },
			//		SDL_Color{ 255, 255, 255, 255 }, 1, 0.0f, false);
			//}

			app->physics->ShapesRender();

			currentDoor = currentDoor->next;
		}
		currentRoom = currentRoom->next;
	}
}
