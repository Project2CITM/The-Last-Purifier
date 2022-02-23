#ifndef __MODULE_SCENE_H__
#define __MODULE_SCENE_H__

#include "Module.h"
#include "External/SDL/include/SDL.h"

#define SCENES_NUM 9

class Scene;

enum SCENES
{
	MAIN_MENU,
	GAME_OVER,
	LEVEL_1,
	LEVEL_2
};

enum class SCENECHANGESTATES 
{
	idle,
	fade_in,
	fade_out
};

class ModuleScene : public Module
{
public:

	// Constructor
	ModuleScene(Application* app, bool start_enabled = true);

	// Destructor
	~ModuleScene();

	bool Init(pugi::xml_node&);

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start();

	UpdateStatus PreUpdate() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	UpdateStatus Update();

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	UpdateStatus PostUpdate();

	UpdateStatus EndUpdate() override;

	bool ChangeCurrentSceneRequest(uint index);

	void GetSaveData(pugi::xml_document& save) override;

	void LoadSaveData(pugi::xml_document& save) override;

	void ResetPlayerSettings();

	bool CleanUp();

	void DebugChangeScene();

private:
	bool StartChangeScene();

	void ChangeSceneSteptoStep();

	//SDL_Texture* fadePanel = nullptr;

	float fade = 0;

	float fadeSpeed = 1.0f;

	SCENECHANGESTATES changeState = SCENECHANGESTATES::idle;

	bool changeSceneRequest = false;
public:

	SCENES currentScene = MAIN_MENU;

	Scene* scenes[SCENES_NUM] = { nullptr };

	bool isChangingScene = false;

	int changeTo = -1;

	//int playerX = 0, playerY = 0;

	//int lastLevel = 0;

	//int characterIndex = 0;
};

#endif // __MODULE_SCENE_H__
