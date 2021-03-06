#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "External/SDL/include/SDL.h"

struct MapObject
{
	int id = 0;
	float rotation = 0;
	iPoint position = { 0,0 };
};

// L03: DONE 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSet
{
	std::string	name = "";
	int	firstgid = 0;
	int margin = 0;
	int	spacing = 0;
	int	tileWidth = 0;
	int	tileHeight = 0;

	SDL_Texture* texture = nullptr;
	int	texWidth = 0;
	int	texHeight = 0;
	int	tilecount = 0;
	int	columns = 0;

	// L04: DONE 7: Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int id) const;
};

// L03: DONE 1: We create an enum for map type, just for convenience,
// NOTE: Platformer game will be of type ORTHOGONAL
enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

//Create a generic structure to hold properties
struct Properties
{
	struct Property
	{
		//...
		std::string name = "";
		int value = 0;
	};
	
	~Properties()
	{
		//...
		ListItem<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.clear();
	}

	// Method to ask for the value of a custom property
	int GetProperty(const char* name, int default_value = 0) const;

	List<Property*> list;
};

// Create a struct for the map layer
struct MapLayer
{
	std::string	name = "";
	int width = 0;
	int height = 0;
	uint* data = 0;

	// L06: DONE 1: Support custom properties
	Properties properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	// L04: DONE 6: Short function to get the value of x,y
	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
	}
};

// L03: DONE 1: Create a struct needed to hold the information to Map node
struct MapData
{
	int width = 0;
	int	height = 0;
	int	tileWidth = 0;
	int	tileHeight = 0;
	SDL_Color backgroundColor = { 0,0,0,0 };
	MapTypes type;
	List<TileSet*> tilesets;

	// L04: DONE 2: Add a list/array of layers to the map
	List<MapLayer*> layers;
};

class ModuleMap : public Module
{
public:

    ModuleMap(bool start_enabled = true);

    // Destructor
    virtual ~ModuleMap();

    // Called before render is available
    bool Init(pugi::xml_node& conf) override;

    // Called each loop iteration
    UpdateStatus PostUpdate() override;

    // Called before quitting
    bool CleanUp() override;

	// Called before quitting
	bool CleanUpScene();

    // Load new map
    bool Load(std::string path);

	// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;

	iPoint MapToWorld(iPoint pos) const;

	// L05: DONE 2: Add orthographic world to map coordinates
	iPoint WorldToMap(int x, int y) const;

	iPoint WorldToMap(iPoint pos) const;

	bool InTileCenter(iPoint worldPos, int tileDimensions = 5) const;

	void LoadLayerMeta();

	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

	void Draw(ListItem<MapLayer*>* mapLayerItem);

private:

	// L03: Methods to load all required map data
	bool LoadMap(pugi::xml_node mapFile);
	bool LoadTileSets(pugi::xml_node mapFile);
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);

	// L04
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadAllLayers(pugi::xml_node mapNode);

	//Load a group of properties 
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	//Pick the right Tileset based on a tile id
	TileSet* GetTilesetFromTileId(int id) const;

public:

    // L03: DONE 1: Add your struct for map info
	MapData mapData;

	List<MapObject> mapObjects;

	bool roof = false;

private:

	std::string currentMap = "null";
    std::string folder = "Assets/Maps/Hub/";
    bool mapLoaded = false;
};

#endif // __MAP_H__