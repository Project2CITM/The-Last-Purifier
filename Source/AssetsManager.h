#ifndef __ASSETSMANAGER_H__
#define __ASSETSMANAGER_H__

#include "Module.h"

#include "External/SDL_image/include/SDL_image.h"
#include "External/SDL_mixer/include/SDL_mixer.h"
#include "External/PhysFS/include/physfs.h"
#include "External/SDL/include/SDL.h"
#include "External/PugiXml/src/pugixml.hpp"

#pragma comment( lib, "External/PhysFS/libx86/physfs.lib" )

class ModuleAssetsManager : public Module
{
public:

	// Constructor
	ModuleAssetsManager();

	// Destructor
	~ModuleAssetsManager();

	// Called before render is available
	bool Init(pugi::xml_node&) override;

	// Called before quitting
	bool CleanUp();

	// Return the bytes of a PhysFS filehandle
	uint LoadData(const char* path, char** buffer) const;

	// Allows you to use pointers to memory instead of files or things such as images or samples
	SDL_Surface* LoadPhysfsTexture(const char* path) const;
	Mix_Chunk* LoadPhysfsFx(const char* path) const;
	Mix_Music* LoadPhysfsMusic(const char* path) const;
	pugi::xml_document* LoadPhysfsXML(const char* path) const;

};

#endif // __ASSETSMANAGER_H__
