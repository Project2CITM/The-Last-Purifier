#include "RoomManager.h"
#include "ModuleRender.h"

void RoomManager::Start()
{
	GenerateMap(10);
	
	CreateDoors();

	doorTopTexture = app->textures->Load("Assets/Maps/TestDoor_top.png");
	doorBotTexture = app->textures->Load("Assets/Maps/TestDoor_bottom.png");

	wallTexture[0] = app->textures->Load("Assets/Maps/wallDoorLeft.png");
	wallTexture[1] = app->textures->Load("Assets/Maps/wallDoorTop.png");
	wallTexture[2] = app->textures->Load("Assets/Maps/TestDoor_bottom.png");

	/*for (int i = 0; i < rooms.count(); ++i)
		rooms[i]->CloseDoors();*/

}

void RoomManager::Update()
{
}

void RoomManager::PostUpdate()
{
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

	//I assume it unloads in renderer?
	doorTopTexture = nullptr;
	doorBotTexture = nullptr;
}

//FUNCTIONS
void RoomManager::GenerateMap(short RoomNumber)
{
	//if the Room number is too big or too little
	if (RoomNumber < 1 || RoomNumber > MAX_ROOMS_ROWS * MAX_ROOMS_COLUMNS)
		return;

	iPoint p;	//create centered room
	p.x = MAX_ROOMS_COLUMNS / 2;
	p.y = MAX_ROOMS_ROWS / 2;
	Room* r = CreateRoom(p, -3);

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
	CreateRoom(bossRoomPos, -1);
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
	while (currentRoom != nullptr) 
	{
		Room* cr = currentRoom->data;

		//Reset wallcolliders
		for (int i = 0; i < 4; ++i)
			cr->wallColliders[i] = nullptr;

		DoorOrientations doorOrient = DoorOrientations::RIGHT;

		//Right Door
		if (roomPositions[cr->roomPosition.x + 1][cr->roomPosition.y] != nullptr && cr->roomPosition.x + 1 < MAX_ROOMS_COLUMNS) 
		{
			doorOrient = DoorOrientations::RIGHT;
			cr->doors.add(new Door(cr->GetDoorPos(doorOrient), cr->GetDoorSize(doorOrient), doorOrient));
		}
		//No Door -> WallCollider
		else {
			doorOrient = DoorOrientations::RIGHT;
			iPoint colSize = cr->GetDoorSize(doorOrient);
			cr->wallColliders[0] = app->physics->CreateRectangle(cr->GetDoorPos(doorOrient), colSize.x, colSize.y, nullptr, b2_staticBody);
		}

		//Bottom Door
		if (roomPositions[cr->roomPosition.x][cr->roomPosition.y + 1] != nullptr && cr->roomPosition.y + 1 < MAX_ROOMS_ROWS) 
		{
			doorOrient = DoorOrientations::BOTTOM;
			cr->doors.add(new Door(cr->GetDoorPos(doorOrient), cr->GetDoorSize(doorOrient), doorOrient));
		}
		//No Door -> WallCollider
		else {
			doorOrient = DoorOrientations::BOTTOM;
			iPoint colSize = cr->GetDoorSize(doorOrient);
			cr->wallColliders[1] = app->physics->CreateRectangle(cr->GetDoorPos(doorOrient), colSize.x, colSize.y, nullptr, b2_staticBody);
		}

		//Left Door
		if (roomPositions[cr->roomPosition.x - 1][cr->roomPosition.y] != nullptr && cr->roomPosition.x - 1 >= 0) 
		{
			doorOrient = DoorOrientations::LEFT;
			cr->doors.add(new Door(cr->GetDoorPos(doorOrient), cr->GetDoorSize(doorOrient), doorOrient));
		}
		//No Door -> WallCollider
		else {
			doorOrient = DoorOrientations::LEFT;
			iPoint colSize = cr->GetDoorSize(doorOrient);
			cr->wallColliders[2] = app->physics->CreateRectangle(cr->GetDoorPos(doorOrient), colSize.x, colSize.y, nullptr, b2_staticBody);
		}

		//Top Door
		if (roomPositions[cr->roomPosition.x][cr->roomPosition.y - 1] != nullptr && cr->roomPosition.y - 1 >= 0) 
		{
			doorOrient = DoorOrientations::TOP;
			cr->doors.add(new Door(cr->GetDoorPos(doorOrient), cr->GetDoorSize(doorOrient), doorOrient));
		}
		//No Door -> WallCollider
		else {
			doorOrient = DoorOrientations::TOP;
			iPoint colSize = cr->GetDoorSize(doorOrient);
			cr->wallColliders[3] = app->physics->CreateRectangle(cr->GetDoorPos(doorOrient), colSize.x, colSize.y, nullptr, b2_staticBody);
		}

		currentRoom = currentRoom->next;
	}
}

Room* RoomManager::CreateRoom(iPoint mapPosition, short mapId)
{
	Room* r = new Room();
	r->roomPosition = mapPosition;
	r->id = mapId;

	//srand(time(NULL));
	folder = "Assets/Maps/map";
	file = ".png";

	std::string s;

	switch (mapId) {
	case -1:
		s = "MiniBoss";	//CHANGE boss room
		break;
	case -2:
		s = "MiniBoss";
		break;
	//case -3:
	//	s = "StartRoom"; //CHANGE starting room
		break;
	default:
		int ran = rand() % 14 + 1;
		s = std::to_string(ran);
		r->id = ran; //Override id for random generated maps
		break;
	}

	folder += s += file;

	r->roomTexture = app->textures->Load(folder);

	//extreure colliders
	mapLoader.ExtractMapColliders(r);

	rooms.add(r);
	roomPositions[mapPosition.x][mapPosition.y] = r;
	return r;
}

//Pseudo-print the rooms
void RoomManager::DrawRooms()
{
	for (int i = 0; i < rooms.count(); ++i)
		rooms[i]->DrawRoom();
}

//Pseudo-print the doors
void RoomManager::DrawDoors()
{
	for (int i = 0; i < rooms.count(); ++i) {
		int k = rooms[i]->doors.count();
		
		for (int j = 0; j < k; ++j) {
			Door* d = rooms[i]->doors[j];
			if (d->orientation == DoorOrientations::TOP)
				app->renderer->AddTextureRenderQueue(doorTopTexture, d->GetPosition() - d->size, { 0,0,0,0 }, TILE_SIZE / 16.0f, 3);
			if (d->orientation == DoorOrientations::BOTTOM)
				app->renderer->AddTextureRenderQueue(doorBotTexture, d->GetPosition() - d->size - iPoint(0, TILE_SIZE), { 0,0,0,0 }, TILE_SIZE / 16.0f, 3);
		}	

		//Draw Walls on Non-doors
		Room* r = rooms[i];
		if (r->wallColliders[0] != nullptr)
			app->renderer->AddTextureRenderQueue(wallTexture[0], 
				r->GetDoorPos(DoorOrientations::RIGHT) - r->GetDoorSize(DoorOrientations::RIGHT) - iPoint(0, TILE_SIZE * 5),
				{ 0,0,0,0 }, TILE_SIZE / 16.0f, 1, 1.0f, 0.0f, SDL_FLIP_HORIZONTAL);

		if (r->wallColliders[1] != nullptr)
			app->renderer->AddTextureRenderQueue(wallTexture[2],
				r->GetDoorPos(DoorOrientations::BOTTOM) - r->GetDoorSize(DoorOrientations::BOTTOM) - iPoint(0, TILE_SIZE),
				{ 0,0,0,0 }, TILE_SIZE / 16.0f, 1, 1.0f);

		if (r->wallColliders[2] != nullptr)
			app->renderer->AddTextureRenderQueue(wallTexture[0], 
				r->GetDoorPos(DoorOrientations::LEFT) - r->GetDoorSize(DoorOrientations::LEFT) - iPoint(0, TILE_SIZE * 5),
				{ 0,0,0,0 }, TILE_SIZE / 16.0f, 1, 1.0f);

		if (r->wallColliders[3] != nullptr)
			app->renderer->AddTextureRenderQueue(wallTexture[1], 
				r->GetDoorPos(DoorOrientations::TOP) - r->GetDoorSize(DoorOrientations::TOP) - iPoint(TILE_SIZE * 4, 0),
				{ 0,0,0,0 }, TILE_SIZE / 16.0f, 1, 1.0f);
	}
}