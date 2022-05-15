#include "RoomManager.h"
#include "ModuleRender.h"
#include "ModuleEvents.h"
#include "ModuleScene.h"
#include "Scene.h"
#include "Trigger.h"
#include "Enemy.h"
#include "Minimap.h"
#include "ModuleInput.h"
#include "GameOverScene.h"

#include "External/Optick/include/optick.h"

void RoomManager::Start()
{
	doorTopTexture = app->textures->Load("Maps/TestDoor_top.png");
	doorBotTexture = app->textures->Load("Maps/TestDoor_bottom.png");
	doorSpikeTexture = app->textures->Load("Maps/wallDoorClosed.png");

	wallTexture[0] = app->textures->Load("Maps/wallDoorLeft.png");
	wallTexture[1] = app->textures->Load("Maps/wallDoorTop.png");
	wallTexture[2] = app->textures->Load("Maps/TestDoor_bottom.png");

	mapLoader = new MapLoader();
	mapSave = new MapSave();
	mapSave->Init();

	GenerateMap(10);

	CreateDoors();

	//Deactivate all colliders
	for (int i = 0; i < rooms.count(); ++i) {
		rooms[i]->DeactivateColliders();
	}

	//Use saved room states
	mapSave->UseRoomStates(&rooms);

	//MiniMap
	miniMap = new MiniMap();
	miniMap->Init(false, &rooms);
	
}

void RoomManager::PreUpdate(iPoint playerPos)
{
	OPTICK_EVENT();
	//Check current room
	Room* r = roomPositions[playerPos.x / (TILE_SIZE * MAX_ROOM_TILES_COLUMNS)][playerPos.y / (TILE_SIZE * MAX_ROOM_TILES_ROWS)];

	//Erase enemies if dead
	for (int i = 0; i < r->enemies.count(); ++i)
	{
		if (r->enemies[i]->pendingToDelete) {
			r->enemies.remove(r->enemies.At(r->enemies.find(r->enemies[i])));
		}
	}
}

void RoomManager::Update(iPoint playerPos)
{
	//MiniMap resize
	miniMap->SetScale((app->input->GetKey(SDL_SCANCODE_TAB) == KEY_REPEAT) ? 2 : 1);
	
	//Check current room
	Room* r = roomPositions[playerPos.x / (TILE_SIZE * MAX_ROOM_TILES_COLUMNS)][playerPos.y / (TILE_SIZE * MAX_ROOM_TILES_ROWS)];

#pragma region //Debug Keys
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		mapSave->ClearSeed();
		mapSave->ClearRoomStates();
	}
	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN) {
		mapSave->SaveRoomStates(&rooms);
	}
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		r->ClearEnemies();
	}
#pragma endregion
	
	//Player is not in any room
	if (r == nullptr) return;

	//Trigger collider (boss room)
	if (exitTrigger->onTriggerEnter && r->done)
	{
		app->events->TriggerEvent(GameEvent::SAVE_GAME);

		mapSave->ClearSeed();
		mapSave->ClearRoomStates();
		
		app->scene->ChangeCurrentSceneRequest(SCENES::WIN);
	}

	//Puzzle 2 completion (pot puzzle)
	if (r->id == -5 && r->enemies.count() == 1)
		r->ClearEnemies();

	//No enemies -> Completed room
	if (r->enemies.count() == 0 && !r->done)
		r->done = true;

	//Close doors when entering
	if (!r->done)
	{
		float enterSensorX = (float)playerPos.x;
		enterSensorX /= (float)(TILE_SIZE * MAX_ROOM_TILES_COLUMNS);
		enterSensorX -= (float)r->roomPosition.x;

		float enterSensorY = (float)playerPos.y;
		enterSensorY /= (float)(TILE_SIZE * MAX_ROOM_TILES_ROWS);
		enterSensorY -= (float)r->roomPosition.y;

		//LOG("PosX: %.2f   PosY: %.2f", enterSensorX, enterSensorY);

		//Horizontal & Verticals limits to close doors
		if (enterSensorX > 0.03f && enterSensorX < 0.97f && enterSensorY > 0.20f && enterSensorY < 0.95f)
		{
			r->CloseDoors();
			r->EnableEnemics();
		}
	}

	//Open Doors when no enemies
	if (r->done && r->closedDoors)
	{
		r->OpenDoors();
		app->events->TriggerEvent(GameEvent::COMPLETE_ROOM);
	}
	//Player has changed room (activate/deactivate colliders)
	if (!r->activeColliders)
		for (int i = 0; i < rooms.count(); ++i)
		{
			if (rooms[i] == r)
			{
				r->ActivateColliders();
			}
			else
			{
				if (rooms[i]->activeColliders)
				{
					rooms[i]->DeactivateColliders();
				}
			}
		}
}

void RoomManager::PostUpdate(iPoint playerPos)
{
	DrawRooms();
	DrawDoors();
	miniMap->MiniMapPrint(iPoint(485, -95), 
		iPoint(playerPos.x / (TILE_SIZE * MAX_ROOM_TILES_COLUMNS),playerPos.y / (TILE_SIZE * MAX_ROOM_TILES_ROWS)));
}

void RoomManager::CleanUp()
{
	for (int i = 0; i < MAX_ROOMS_COLUMNS; ++i) {
		for (int j = 0; j < MAX_ROOMS_ROWS; j++) {
			roomPositions[i][j] = nullptr;
		}
	}

	for (int i = 0; i < rooms.count(); i++)
	{
		rooms[i]->CleanUp();
	}
	rooms.clearPtr();

	RELEASE(mapLoader);
	RELEASE(miniMap);
	RELEASE(mapSave);

	doorTopTexture = nullptr;
	doorBotTexture = nullptr;
	doorSpikeTexture = nullptr;

	if (app->Exiting()) return;

	app->scene->scenes[app->scene->currentScene]->DestroyGameObject(exitTrigger);
}

//FUNCTIONS
void RoomManager::GenerateMap(short RoomNumber)
{
	//if the Room number is too big or too little
	if (RoomNumber < 1 || RoomNumber > MAX_ROOMS_ROWS * MAX_ROOMS_COLUMNS)
		return;

	//Seed
	if (mapSave->CurrentSeed() == 0) {
		mapSave->GenerateSeed();
	}
	else {
		mapSave->UseCurrentSeed();
	}

	iPoint p;	//create centered room
	p.x = MAX_ROOMS_COLUMNS / 2;
	p.y = MAX_ROOMS_ROWS / 2;
	Room* r = CreateRoom(p, -3);

	//Create all rooms except the first and Boss room
	while (RoomNumber > 2) 
	{
		iPoint pos = r->roomPosition;
		if (CheckAdjacentSpace(r) > 0)
		{	//there is space next to the current room to spawn another
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
			if ((pos.x < MAX_ROOMS_COLUMNS && pos.x >= 0 && pos.y < MAX_ROOMS_ROWS && pos.y >= 0) && roomPositions[pos.x][pos.y] == nullptr) 
			{
				r = CreateRoom(iPoint(pos.x, pos.y));
				--RoomNumber;
			}
		}
		else {	//room is trapped, chose another one
			r = rooms.At(rand() % rooms.count())->data;
		}
	}

	//Generate MiniBoss and PuzzleRooms
	int id = -2;
	bool f = true;
	while (f) 
	{
		//Choose random room to start
		iPoint p(rand() % MAX_ROOMS_COLUMNS, rand() % MAX_ROOMS_ROWS);
		if (roomPositions[p.x][p.y] == nullptr && CheckAdjacentSpace(p) == 3)
		{
			CreateRoom(p, id);
			
			switch (id) 
			{
			case -2:	//MiniBoss room
				id = -4;	//First Puzzle
				break;

			case -6:	//Last Puzzle
				f = false;
				break;

			default:
				id--;
				break;
			}
		}
	}
	
	//BOSS ROOM
	iPoint bossRoomPos = iPoint(-1, -1);	//initialize the Boss room position
	iPoint startRoomPos = rooms[0]->roomPosition;
	int adjacentSpaces = 3;	//Blank spaces that the Boss room should have

	do {	//check all rooms that have 3 spaces left
		for (int i = 0; i < MAX_ROOMS_COLUMNS; ++i) 
		{
			for (int j = 0; j < MAX_ROOMS_ROWS; ++j) 
			{
				//if the boss room is not initialized, do it
				if (bossRoomPos != iPoint(-1, -1)) 
				{
					//if the room is nearer to the start than the last one, don't check
					if (((bossRoomPos.x - startRoomPos.x) * (bossRoomPos.x - startRoomPos.x) + (bossRoomPos.y - startRoomPos.y) * (bossRoomPos.y - startRoomPos.y))
						< ((i - startRoomPos.x) * (i - startRoomPos.x) + (j - startRoomPos.y) * (j - startRoomPos.y))) 
					{
						//check the space is not ocupied, and the number of spaces adjacent to it is correct
						if (CheckAdjacentSpace(iPoint(i, j)) == adjacentSpaces && roomPositions[i][j] == nullptr)
						{
							bossRoomPos = iPoint(i, j);
						}
					}
				}
				else 
				{
					if (CheckAdjacentSpace(iPoint(i, j)) == adjacentSpaces && roomPositions[i][j] == nullptr) 
					{
						bossRoomPos = iPoint(i, j);
					}
				}
			}
		}
		adjacentSpaces--;
	} while (bossRoomPos == iPoint(-1, -1));

	CreateRoom(bossRoomPos, -1);

	//Exit trigger in boss room
	iPoint triggerPos = bossRoomPos;
	triggerPos.x *= MAX_ROOM_TILES_COLUMNS;
	triggerPos.y *= MAX_ROOM_TILES_ROWS;
	triggerPos += iPoint(31, 11);
	triggerPos *= TILE_SIZE;
	exitTrigger = new Trigger(triggerPos, 2 * TILE_SIZE);
	b2Filter filter;
	filter.categoryBits = app->physics->TRIGGER_LAYER;
	filter.maskBits = app->physics->PLAYER_LAYER;
	exitTrigger->pBody->body->GetFixtureList()->SetFilterData(filter);
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

	if (x + 1 >= 0 && x + 1 < MAX_ROOMS_COLUMNS) 
	{
		if (roomPositions[x + 1][y] == nullptr) spaces++;
	}
	else 
	{
		spaces++;
	}

	if (y + 1 >= 0 && y + 1 < MAX_ROOMS_ROWS) 
	{
		if (roomPositions[x][y + 1] == nullptr) spaces++;
	}
	else 
	{
		spaces++;
	}

	if (x - 1 >= 0 && x - 1 < MAX_ROOMS_COLUMNS) 
	{
		if (roomPositions[x - 1][y] == nullptr) spaces++;
	}
	else 
	{
		spaces++;
	}

	if (y - 1 >= 0 && y - 1 < MAX_ROOMS_ROWS)
	{
		if (roomPositions[x][y - 1] == nullptr) spaces++;
	}
	else 
	{
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

	std::string folder = "Assets/Maps/map";
	std::string file = ".png";

	std::string s;

	switch (mapId) {
	case -1:
		s = "Boss";
		break;
	case -2:
		s = "MiniBoss";
		break;
	case -3:
		s = "Start";
		break;
	case -4:
		s = "Puzzle1";
		break;
	case -5:
		s = "Puzzle2";
		break;
	case -6:
		s = "Puzzle3";
		break;
	default:
		int ran = rand() % 14 + 1;
		s = std::to_string(ran);
		r->id = ran; //Override id for random generated maps
		break;
	}

	folder += s += file;

	r->roomTexture = app->textures->Load(folder);

	//Extract colliders & enemies
	mapLoader->ExtractMapInfo(r);

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
		Room* r = rooms[i];
		int k = r->doors.count();

		for (int j = 0; j < k; ++j) {
			Door* d = r->doors[j];
			if (d->orientation == DoorOrientations::TOP)
				app->renderer->AddTextureRenderQueue(doorTopTexture, d->GetPosition() - d->size, { 0,0,0,0 }, TILE_SIZE / 16.0f, 3);
			if (d->orientation == DoorOrientations::BOTTOM)
				app->renderer->AddTextureRenderQueue(doorBotTexture, d->GetPosition() - d->size - iPoint(0, TILE_SIZE), { 0,0,0,0 }, TILE_SIZE / 16.0f, 3);
		}

		//Draw Spikes
		if (r->closedDoors) {
			for (int j = 0; j < k; ++j) {
				Door* d = r->doors[j];
				iPoint p = d->GetPosition() - d->size;
				switch (d->orientation) {
				case DoorOrientations::BOTTOM:
					p += iPoint(0, 1) * TILE_SIZE;
				case DoorOrientations::TOP:
					p.y -= TILE_SIZE / 2;
					app->renderer->AddTextureRenderQueue(doorSpikeTexture, p + iPoint(0, 2) * TILE_SIZE, { 0,0,0,0 }, TILE_SIZE / 16.0f, 1);
					app->renderer->AddTextureRenderQueue(doorSpikeTexture, p + iPoint(1, 2) * TILE_SIZE, { 0,0,0,0 }, TILE_SIZE / 16.0f, 1);
					app->renderer->AddTextureRenderQueue(doorSpikeTexture, p + iPoint(2, 2) * TILE_SIZE, { 0,0,0,0 }, TILE_SIZE / 16.0f, 1);
					break;
				case DoorOrientations::LEFT:
				case DoorOrientations::RIGHT:
					app->renderer->AddTextureRenderQueue(doorSpikeTexture, p + iPoint(0, -3) * TILE_SIZE, { 0,0,0,0 }, TILE_SIZE / 16.0f, 1);
					app->renderer->AddTextureRenderQueue(doorSpikeTexture, p + iPoint(0, -2) * TILE_SIZE, { 0,0,0,0 }, TILE_SIZE / 16.0f, 1);
					app->renderer->AddTextureRenderQueue(doorSpikeTexture, p + iPoint(0, -1) * TILE_SIZE, { 0,0,0,0 }, TILE_SIZE / 16.0f, 1);
					break;
				}
			}
		}

		//Draw Walls on Non-doors
		if (r->wallColliders[0] != nullptr)
			app->renderer->AddTextureRenderQueue(wallTexture[0],
				r->GetDoorPos(DoorOrientations::RIGHT) - r->GetDoorSize(DoorOrientations::RIGHT) - iPoint(0, TILE_SIZE * 4),
				{ 0,0,0,0 }, TILE_SIZE / 16.0f, 1, 1.0f, 0.0f, SDL_FLIP_HORIZONTAL);

		if (r->wallColliders[1] != nullptr)
			app->renderer->AddTextureRenderQueue(wallTexture[2],
				r->GetDoorPos(DoorOrientations::BOTTOM) - r->GetDoorSize(DoorOrientations::BOTTOM) - iPoint(0, TILE_SIZE),
				{ 0,0,0,0 }, TILE_SIZE / 16.0f, 1, 1.0f);

		if (r->wallColliders[2] != nullptr)
			app->renderer->AddTextureRenderQueue(wallTexture[0],
				r->GetDoorPos(DoorOrientations::LEFT) - r->GetDoorSize(DoorOrientations::LEFT) - iPoint(0, TILE_SIZE * 4),
				{ 0,0,0,0 }, TILE_SIZE / 16.0f, 1, 1.0f);

		if (r->wallColliders[3] != nullptr)
			app->renderer->AddTextureRenderQueue(wallTexture[1],
				r->GetDoorPos(DoorOrientations::TOP) - r->GetDoorSize(DoorOrientations::TOP) - iPoint(TILE_SIZE * 4, 0),
				{ 0,0,0,0 }, TILE_SIZE / 16.0f, 1, 1.0f);
	}
}