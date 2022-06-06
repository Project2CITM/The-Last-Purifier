#include "MapLoader.h"
#include "Ghoul.h"
#include "Kaboom.h"
#include "Worm.h"
#include "Slime.h"
#include "Column.h"

void MapLoader::ExtractMapInfo(Room* r)
{
	LoadMap(r->id);
	CreateColliders(r);
	CreateEnemies(r);
}

void MapLoader ::LoadMap(short id)
{
	//Create path from id
	string filePath = "Assets/Maps/map";
	if (id > 0) {
		filePath += to_string(id);
	}
	else {
		switch (id) {
		case -1:
			filePath += "Boss";
			break;
		case -2:
			filePath += "MiniBoss";
			break;
		case -3:
			filePath += "Start";
			break;
		case -4:
			filePath += "Puzzle1";
			break;
		case -5:
			filePath += "Puzzle2";
			break;
		case -6:
			filePath += "Puzzle3";
			break;
		}
	}
	filePath += ".tmx";

	//Pick xml from path
	pugi::xml_parse_result result = mapDoc.load_file(filePath.c_str());
	if (result == NULL)
		LOG("Could not load (map) xml file: %s. pugi error: %s", filePath, result.description())
	else
		mapNode = mapDoc.child("map");
}

void MapLoader::CreateColliders(Room* r)
{
	r->mapInfo.resize(MAX_ROOM_TILES_ROWS);

	for (int i = 0; i < MAX_ROOM_TILES_ROWS; i++) r->mapInfo[i].resize(MAX_ROOM_TILES_COLUMNS);

	//Find collision layer
	pugi::xml_node colliderLayer = mapNode.child("layer");
	for (colliderLayer; colliderLayer != NULL; colliderLayer = colliderLayer.next_sibling()) {
		string name = colliderLayer.attribute("name").as_string();
		if ( name == "Colisions") break;
	}

	//Get node tiles (with gid attributes)
	pugi::xml_node tile = colliderLayer.first_child().first_child();

	for (int i = 0; i < MAX_ROOM_TILES_ROWS; ++i) 
	{
		for (int j = 0; j < MAX_ROOM_TILES_COLUMNS; ++j, tile = tile.next_sibling())
		{
			if(tile.first_attribute().as_int() < 2561) continue;

			//Normal Colision
			if (tile.first_attribute().as_int() == 2561)
			{
				r->colliders.add(app->physics->CreateRectangle(iPoint(
					r->roomPosition.x * MAX_ROOM_TILES_COLUMNS * TILE_SIZE + j * TILE_SIZE + TILE_SIZE / 2,
					r->roomPosition.y * MAX_ROOM_TILES_ROWS * TILE_SIZE + i * TILE_SIZE + TILE_SIZE / 2),
					TILE_SIZE / 2, TILE_SIZE / 2, nullptr, b2_staticBody));				
			}
			//Void Collision
			else if (tile.first_attribute().as_int() == 2562) 
			{
				PhysBody* holeCollision = app->physics->CreateRectangle(iPoint(
					r->roomPosition.x * MAX_ROOM_TILES_COLUMNS * TILE_SIZE + j * TILE_SIZE + TILE_SIZE / 2,
					r->roomPosition.y * MAX_ROOM_TILES_ROWS * TILE_SIZE + i * TILE_SIZE + TILE_SIZE / 2),
					TILE_SIZE / 2, TILE_SIZE / 2, nullptr, b2_staticBody);
				b2Filter filter;
				filter.categoryBits = app->physics->PROJECTILE_LAYER;
				holeCollision->body->GetFixtureList()[0].SetFilterData(filter);
				r->colliders.add(holeCollision);
			}

			r->mapInfo[i][j] = 1;
		}
	}
}

void MapLoader::CreateEnemies(Room* r)
{	
	//r->enemies.add(new EnemyDummy(iPoint(
	//	r->roomPosition.x * TILE_SIZE * MAX_ROOM_TILES_COLUMNS + (TILE_SIZE * MAX_ROOM_TILES_COLUMNS) / 2,
	//	r->roomPosition.y * TILE_SIZE * MAX_ROOM_TILES_ROWS + (TILE_SIZE * MAX_ROOM_TILES_ROWS) / 2 )));
	
	//r->enemies.add(new Ghoul(iPoint(
	//	r->roomPosition.x * TILE_SIZE * MAX_ROOM_TILES_COLUMNS + (TILE_SIZE * MAX_ROOM_TILES_COLUMNS) / 2,
	//	r->roomPosition.y * TILE_SIZE * MAX_ROOM_TILES_ROWS + (TILE_SIZE * MAX_ROOM_TILES_ROWS) / 2)));

	//Find enemy layer
	pugi::xml_node enemyLayer = mapNode.child("layer");
	for (enemyLayer; enemyLayer != NULL; enemyLayer = enemyLayer.next_sibling()) 
	{
		string name = enemyLayer.attribute("name").as_string();
		if (name == "Enemics") break;
	}

	//Get node tiles (with gid attributes)
	pugi::xml_node tile = enemyLayer.first_child().first_child();
	for (int i = 0; i < MAX_ROOM_TILES_ROWS; ++i) 
	{
		for (int j = 0; j < MAX_ROOM_TILES_COLUMNS; ++j) 
		{
			int gid = tile.first_attribute().as_int();
			
			if(gid == 2562)
			{
				int rand = std::rand() % 15;
				Enemy* g1 = nullptr;

				switch (rand)
				{
				case 0:
				case 1:
				case 2:
				case 3:
					g1 = new Ghoul(iPoint(
						r->roomPosition.x * MAX_ROOM_TILES_COLUMNS * TILE_SIZE + j * TILE_SIZE + TILE_SIZE / 2,
						r->roomPosition.y * MAX_ROOM_TILES_ROWS * TILE_SIZE + i * TILE_SIZE + TILE_SIZE / 2),r);
					r->enemies.add(g1);
					g1->enable = false;
					break;
				case 4:
					g1 = new Ghoul(iPoint(
					r->roomPosition.x * MAX_ROOM_TILES_COLUMNS * TILE_SIZE + j * TILE_SIZE + TILE_SIZE / 2,
					r->roomPosition.y * MAX_ROOM_TILES_ROWS * TILE_SIZE + i * TILE_SIZE + TILE_SIZE / 2), r);
					r->enemies.add(g1);
					g1->enable = false;
					break;
				case 5:
					g1 = new Kaboom(iPoint(
						r->roomPosition.x * MAX_ROOM_TILES_COLUMNS * TILE_SIZE + j * TILE_SIZE + TILE_SIZE / 2,
						r->roomPosition.y * MAX_ROOM_TILES_ROWS * TILE_SIZE + i * TILE_SIZE + TILE_SIZE / 2),r);
					r->enemies.add(g1);
					g1->enable = false;
					break;
				case 6:
					g1 = new Worm(iPoint(
						r->roomPosition.x * MAX_ROOM_TILES_COLUMNS * TILE_SIZE + j * TILE_SIZE + TILE_SIZE / 2,
						r->roomPosition.y * MAX_ROOM_TILES_ROWS * TILE_SIZE + i * TILE_SIZE + TILE_SIZE / 2));
					r->enemies.add(g1);
					g1->enable = false;
					break;
				case 7:
					g1 = new Ghoul(iPoint(
						r->roomPosition.x * MAX_ROOM_TILES_COLUMNS * TILE_SIZE + j * TILE_SIZE + TILE_SIZE / 2,
						r->roomPosition.y * MAX_ROOM_TILES_ROWS * TILE_SIZE + i * TILE_SIZE + TILE_SIZE / 2), r);
					r->enemies.add(g1);
					g1->enable = false;
					break;
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
					break;
				}
				

				
			}
			else if (gid == 2561) 
			{
				//TODO: spawn boss
			}
			else if (gid == 2563) 
			{
				//TODO: spawn laser enemy
				Enemy* g1 = new Slime(iPoint(
					r->roomPosition.x * MAX_ROOM_TILES_COLUMNS * TILE_SIZE + j * TILE_SIZE + TILE_SIZE / 2,
					r->roomPosition.y * MAX_ROOM_TILES_ROWS * TILE_SIZE + i * TILE_SIZE + TILE_SIZE / 2));
				r->enemies.add(g1);
				g1->enable = false;			
			}
			else if (gid == 2564) 
			{
				//TODO: spawn column
				Enemy* e = new Column(iPoint(r->roomPosition.x * MAX_ROOM_TILES_COLUMNS * TILE_SIZE + j * TILE_SIZE + TILE_SIZE / 2,
					r->roomPosition.y * MAX_ROOM_TILES_ROWS * TILE_SIZE + i * TILE_SIZE + TILE_SIZE / 2));
				r->enemies.add(e);
				e->enable = false;
			}
			else if (gid == 2565)
			{
				//TODO: spawn mini boss
				Enemy* mb = new Ghoul(iPoint(
					r->roomPosition.x * MAX_ROOM_TILES_COLUMNS * TILE_SIZE + j * TILE_SIZE + TILE_SIZE / 2,
					r->roomPosition.y * MAX_ROOM_TILES_ROWS * TILE_SIZE + i * TILE_SIZE + TILE_SIZE / 2), r, true);
				r->enemies.add(mb);
				mb->enable = false;
			}

			tile = tile.next_sibling();
		}
	}
}

