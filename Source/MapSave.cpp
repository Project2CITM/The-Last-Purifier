#include "MapSave.h"

void MapSave::Init()
{
	//Pick xml
	pugi::xml_parse_result result = mapDoc.load_file("SaveMap.xml");
	if (result == NULL)
		LOG("Could not load (SaveMap) xml file. Pugi error: %s", result.description())
	else
		mapNode = mapDoc.child("map");
}

int MapSave::CurrentSeed()
{
	return mapNode.attribute("seed").as_int();
}

int MapSave::GenerateSeed()
{
	int seed = time(NULL);
	srand(seed);
	mapNode.attribute("seed").set_value(seed);
	mapDoc.save_file(MAPSAVE_NAME);
	return seed;
}

void MapSave::UseCurrentSeed()
{
	srand(mapNode.attribute("seed").as_int());
}

void MapSave::UseSeed(int seed)
{
	srand(seed);
}

void MapSave::ClearSeed()
{
	mapNode.attribute("seed").set_value(0);
	mapDoc.save_file(MAPSAVE_NAME);
}

void MapSave::SaveRoomStates()
{
}

void MapSave::ClearRoomStates()
{
}
