#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "List.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleScene.h"
#include "ModuleUI.h"

#include "Timer.h"

#define CONFIG_FILENAME		"config.xml"
#define SAVE_STATE_FILENAME "save_game.xml"

#define FPS 60
#define FRAME_TIME (1.0/FPS)

class Application
{
public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleTextures* textures = nullptr;
	ModuleInput* input = nullptr;
	ModuleAudio* audio = nullptr;
	ModulePhysics* physics = nullptr;
	ModuleScene* scene = nullptr;
	ModuleUI* ui = nullptr;

	Timer globalTime;

	//bool isDebug = false;

	std::string title = "";
	std::string organization = "";
	float averageFps = 0.0f;
	float dt = 0.0f;
	float frameTime = 1.0f / 60.0f;
	bool vsync = false;

	mutable bool saveGameRequested = false;
	bool loadGameRequested = false;

	pugi::xml_node config;
	pugi::xml_node configApp;
	pugi::xml_document configF;
	pugi::xml_document saveF;

private:
	List<Module*> list_modules;

public:
	Application();
	~Application();

	bool Init();
	UpdateStatus Update();
	bool CleanUp();

	void LoadGameRequest();
	void SaveGameRequest() const;

	void ShowTime();

	const char* GetTitle() const;
	const char* GetOrganization() const;

	pugi::xml_node LoadConfig(pugi::xml_document&) const;
	bool LoadSaveFile();

	void ExitGame();

	bool FullScreenDesktop = true;

private:
	// Load / Save
	bool LoadGame();
	bool SaveGame();

	void UpdateTitle();

	float deltaTime = 0, sleepTime = 0;

	void AddModule(Module* mod);

	bool isExiting = false;
};

#endif // !__APPLICATION_H__