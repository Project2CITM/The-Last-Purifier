#include "Room.h"
#include "Enemy.h"
#include <stack>
#include <vector>
#include <queue>

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
	/*
	// Struct just for this func
	struct PathNode
	{
		iPoint pos;

		int priority = 0;

		int costIndex = 0;

		int lastIndex = 0;

		int currentIndex = 0;

		void Init(iPoint pos, int priority, int costIndex, int lastIndex)
		{
			this->pos = pos;
			this->priority = priority;
			this->costIndex = costIndex;
			this->lastIndex = lastIndex;
		}

		bool operator > (PathNode x) const //const²»ÄÜÉÙ
		{
			return this->priority > x.priority;
		}
	};
	*/
	// Struct just for this func
	struct PathNode
	{
		iPoint pos;

		iPoint dir; // Direction when coming

		int lastIndex; // last path index

		int g_cost; // Current price

		int h_cost; // Estimated price

		int total_cost; //  Current price + Estimated price

		void Init(iPoint pos, int lastIndex, int g_cost, int h_cost, iPoint dir)
		{
			this->pos = pos;
			this->lastIndex = lastIndex;
			this->g_cost = g_cost;
			this->h_cost = h_cost;
			this->total_cost = h_cost + g_cost;
			this->dir = dir;
		}
	};

	src = GetTilePos(src);

	des = GetTilePos(des);
	/*
	priority_queue< PathNode, vector<PathNode>, greater<PathNode>> openNode;

	vector<pair<iPoint, int>> costSoFar;

	vector<PathNode> closeNode;

	costSoFar.push_back(pair<iPoint, int>{src, 0});

	PathNode firstNode;

	firstNode.Init(src, 0, 0, 0);

	openNode.push(firstNode);

	while (!openNode.empty())
	{
		PathNode currentNode = openNode.top();

		openNode.pop();

		if (currentNode.pos == des) break;

		iPoint dir[4] = { {0,1}, {0,-1}, {1,0}, {-1,0} };

		for (int i = 0; i < 4; i++)
		{
			iPoint next = currentNode.pos + dir[i];

			int newCost = costSoFar[currentNode.costIndex].second + 1;

			int validCost = 0;

			int costIndex = 0;

			for (int i = 0; i < costSoFar.size(); i++)
			{
				if (next == costSoFar[i].first)
				{
					if (newCost < costSoFar[i].second)
					{
						costSoFar[i].second = newCost;

						validCost = 1;

						costIndex = i;
					}

					break;
				}
				if (i == costSoFar.size() - 1)
				{
					costSoFar.push_back(pair<iPoint, int>{next, newCost});

					validCost = 2;
				}
			}

			if (validCost > 0)
			{
				int priority = newCost + next.DistanceManhattan(des);

				PathNode nextPath;

				if (validCost == 1)
				{
					nextPath.Init(next, priority, costIndex, currentNode.currentIndex);
				}
				else if(validCost == 2)
				{
					nextPath.Init(next, priority, costSoFar.size() - 1, currentNode.currentIndex);
				}

				openNode.push(nextPath);

				if (closeNode.size() <= 0)
				{
					currentNode.currentIndex = closeNode.size();
					closeNode.push_back(currentNode);
					break;
				}
				for (int i = 0; i < closeNode.size(); i++)
				{
					if (closeNode[i].pos == currentNode.pos)
					{
						closeNode[i] = currentNode;
						break;
					}
					if (i == closeNode.size() - 1)
					{
						currentNode.currentIndex = closeNode.size();
						closeNode.push_back(currentNode);
					}
				}			
			}
		}
	}

	if (closeNode.size() != 0)
	{
		for (int i = closeNode.size() - 1; i >= 0; i = closeNode[i].lastIndex)
		{
			if (closeNode[i].lastIndex == 0)
			{
				return (closeNode[i].pos - src);
			}
		}

		//return (src - closeNode[1].pos);
	}*/

	vector<PathNode> openNode;

	vector<PathNode> closeNode;

	PathNode firstNode;

	int distance = src.DistanceManhattan(des);
	
	firstNode.Init(src, -1, 0, distance, { 0, 0 });

	openNode.push_back(firstNode);

	while (!openNode.empty())
	{
		// index  total cost
		pair<int, PathNode> currentNode = { 0, openNode[0]};

		// Find the less cost Node
		for (int i = 0; i < openNode.size(); i++)
		{
			if (currentNode.second.total_cost > openNode[i].total_cost)
			{
				currentNode.first = i;
				currentNode.second = openNode[i];
			}
		}

		openNode.erase(openNode.begin() + currentNode.first);

		if (currentNode.second.h_cost == 0) break;

		iPoint dir[4] = { {0,1}, {0,-1}, {1,0}, {-1,0} };

		for (int i = 0; i < 4; i++)
		{
			iPoint checkPos = currentNode.second.pos + dir[i];
			
			bool existInNode = false;

			for (int j = 0; j < closeNode.size(); j++) if (closeNode[j].pos == checkPos) existInNode = true;		

			for (int j = 0; j < openNode.size(); j++) if (openNode[j].pos == checkPos) existInNode = true;

			if (existInNode) continue;

			if (IsWalkable(checkPos))
			{				
				int estimateCost = checkPos.DistanceManhattan(des);

				PathNode p;

				p.Init(checkPos, closeNode.size(), currentNode.second.g_cost + 1, estimateCost, (checkPos - currentNode.second.pos));

				openNode.push_back(p);
			}
		}
		closeNode.push_back(currentNode.second);
	}

	for (int i = closeNode.size() - 1; i >= 0; i = closeNode[i].lastIndex)
	{
		printf("\n--------------------------------------------------------------\n");
		printf("N: %d   X: %d\t Y: %d\n", i, closeNode[i].pos.x, closeNode[i].pos.y);

		if (closeNode[i].lastIndex == 0)
		{
			// printf("Node num = %d\n", closeNode.size());
			return (closeNode[i].dir);
		}
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
	if (checkPos.x < 0 || checkPos.y < 0 || checkPos.x >= mapInfo[0].size() || checkPos.y >= mapInfo.size()) return false;

	if (mapInfo[checkPos.y][checkPos.x] == 0) return true;

	return false;
}
