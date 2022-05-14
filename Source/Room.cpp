#include "Room.h"
#include "Enemy.h"
#include <stack>
#include <vector>

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
	for (int i = 0; i < colliders.count(); ++i) 
	{
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
		enemies[i]->CleanUp();
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

iPoint Room::PathFindingAStar(iPoint src, iPoint des)
{
	// Struct just for this func
	struct PathNode
	{
		iPoint pos;
		int previousIndex = 0;
		int currentCost = 0;
		int estimateCost = 0;
		int totalCost = 0;

		void Init(iPoint pos, int previousIndex, int currentCost, int estimateCost)
		{
			this->pos = pos;
			this->previousIndex = previousIndex;
			this->currentCost = currentCost;
			this->estimateCost = estimateCost;
			this->totalCost = currentCost + estimateCost;
		}
	};

	src = GetTilePos(src);

	des = GetTilePos(des);

	// Get distance Manhattan
	int distance = src.DistanceManhattan(des);

	vector<PathNode> openNode;
	vector<PathNode> closeNode;

	PathNode firstNode;
	firstNode.Init(src, -1 , 0, distance);

	openNode.push_back(firstNode);

	while (openNode.size() > 0)
	{
		pair<int, PathNode> currentNode;

		currentNode.first = 0; // index
		
		currentNode.second = openNode[0]; // total cost

		// Find the less cost Node
		for (int i = 0; i < openNode.size(); i++)
		{
			if (currentNode.second.totalCost > openNode[i].totalCost)
			{
				currentNode.first = i;
				currentNode.second = openNode[i];
			}
		}

		openNode.erase(openNode.begin() + currentNode.first);

		iPoint dir[4] = { {0,1}, {0,-1}, {1,0}, {-1,0} };

		for (int i = 0; i < 4; i++)
		{
			iPoint checkPos = currentNode.second.pos + dir[i];
			
			bool existInCloseNode = false;

			for (int j = 0; j < closeNode.size(); j++) if (closeNode[j].pos == checkPos) existInCloseNode = true;		

			if (existInCloseNode) continue;

			if (IsWalkable(checkPos))
			{				
				int estimateCost = checkPos.DistanceManhattan(des);

				PathNode p;

				p.Init(checkPos, closeNode.size(), currentNode.second.currentCost + 1, estimateCost);

				openNode.push_back(p);

				// If we find destination, return the direction
				if (estimateCost == 0)
				{
					return (dir[i] * -1);

					for (int i = closeNode.size(); i >= 0; i = closeNode[i].previousIndex)
					{
						if (closeNode[i].previousIndex == 0)
						{
							return (closeNode[i].pos - closeNode[0].pos);
						}
					}
				}
			}
		}

		closeNode.push_back(currentNode.second);
	}

	return { 0,0 };
}

iPoint Room::GetTilePos(iPoint worldPos)
{
	iPoint roomTilePos = roomPosition;

	roomTilePos.x *= MAX_ROOM_TILES_COLUMNS;

	roomTilePos.y *= MAX_ROOM_TILES_ROWS;

	iPoint checkerTilePos = worldPos;

	checkerTilePos.x /= TILE_SIZE;

	checkerTilePos.y /= TILE_SIZE;

	return (checkerTilePos - roomTilePos);
}

bool Room::IsWalkable(iPoint checkPos)
{
	if (mapInfo[checkPos.y][checkPos.x] == 0) return true;

	return false;
}
