#include "Room.h"
#include "Enemy.h"

void Room::CloseDoors()
{
	if (closedDoors) return;
	
	for (int i = 0; i < doors.count(); i++) doors[i]->CreateCollider();
	closedDoors = true;
}

void Room::OpenDoors()
{
	if (!closedDoors) return;

	for (int i = 0; i < doors.count(); i++) doors[i]->DestroyCollider();
	closedDoors = false;
}

void Room::ActivateColliders()
{
	for (int i = 0; i < colliders.count(); ++i) {
		colliders[i]->body->SetActive(true);
	}
	activeColliders = true;
}

void Room::DeactivateColliders()
{
	for (int i = 0; i < colliders.count(); ++i) {
		colliders[i]->body->SetActive(false);
	}
	activeColliders = false;
}

void Room::DrawRoom()
{
	app->renderer->AddTextureRenderQueue(roomTexture, 
		iPoint(roomPosition.x * MAX_ROOM_TILES_COLUMNS, roomPosition.y * MAX_ROOM_TILES_ROWS) * TILE_SIZE, 
		{0,0,0,0}, TILE_SIZE / 16.0f);
}

void Room::CleanUp()
{
	for (int i = 0; i < doors.count(); ++i) doors[i]->pendingToDelete = true;
	
	for (int i = 0; i < 4; ++i) {
		delete wallColliders[i];
		wallColliders[i] = nullptr;
	}

	ClearEnemies();

	colliders.clearPtr();
}

void Room::ClearEnemies()
{
	for (int i = 0; i < enemies.count(); i++)
	{
		enemies[i]->Enemy::Die(false, false);
	}

	enemies.clear();
}

void Room::EnableEnemics()
{
	for (int i = 0; i < enemies.count(); i++)
	{
		enemies[i]->enable = true;
	}
}

iPoint Room::GetDoorPos(DoorOrientations orient)
{
	iPoint ret = roomPosition;

	ret.x *= MAX_ROOM_TILES_COLUMNS;

	ret.y *= MAX_ROOM_TILES_ROWS;

	ret += doorPos[(int)orient];

	ret *= TILE_SIZE;

	ret += GetDoorSize(orient);

	return ret;
}

iPoint Room::GetDoorSize(DoorOrientations orient)
{
	iPoint ret = doorSize[(int)orient] * (TILE_SIZE/2);

	return ret;
}