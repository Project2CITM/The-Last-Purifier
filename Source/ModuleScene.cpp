#include <time.h>
#include "ModuleScene.h"
#include "ModuleAudio.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "TestScene.h"
#include "HubScene.h"
#include "MainMenu.h"
#include "LogoScene.h"
#include "GameOverScene.h"
#include "WinScreen.h"

ModuleScene::ModuleScene(bool start_enabled) : Module(start_enabled)
{
	name = "scenes";

	scenes[LOGO] = new LogoScene();
	scenes[MAIN_MENU] = new MainMenu();
	scenes[HUB] = new HubScene();
	scenes[LEVEL_1] = new TestScene();
	scenes[GAME_OVER] = new GameOverScene();
	scenes[WIN] = new WinScene();

	currentScene = HUB;
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init(pugi::xml_node& config)
{
	this->config = config;

	return true;
}

bool ModuleScene::Start()
{
	bool ret = true;

	if (scenes[currentScene] == nullptr)
	{
		return ret;
	}

	scenes[currentScene]->Start();

	//Mix_VolumeMusic(app->saveF.child("game_state").child("settings").attribute("music").as_float() * 60);

	//fadePanel = app->textures->Load("Assets/textures/Menu/fadePanel.png");

	return ret;
}

UpdateStatus ModuleScene::PreUpdate()
{
	if (scenes[currentScene] == nullptr)
	{
		return UpdateStatus::UPDATE_CONTINUE;
	}

	scenes[currentScene]->PreUpdate();

	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleScene::Update()
{
	if (scenes[currentScene] == nullptr)
	{
		return UpdateStatus::UPDATE_CONTINUE;
	}

	if (changeSceneWaitFrames > 0)changeSceneWaitFrames--;

	if(isChangingScene && changeSceneWaitFrames <= 0)
	{
		ChangeSceneSteptoStep();

		return UpdateStatus::UPDATE_CONTINUE;
	}

	if (!scenes[currentScene]->Update())
	{
		return UpdateStatus::UPDATE_STOP;
	}

	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleScene::PostUpdate()
{
	if (scenes[currentScene] == nullptr)
	{
		return UpdateStatus::UPDATE_CONTINUE;
	}

	scenes[currentScene]->PostUpdate();

	if (fade != 0) app->renderer->AddRectRenderQueue(SDL_Rect{ 0,0,(int)app->window->width,(int)app->window->height }, SDL_Color{ 0,0,0,(Uint8)fade }, true, 4, 200, 0);

	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleScene::EndUpdate()
{
	if (scenes[currentScene] == nullptr)
	{
		return UpdateStatus::UPDATE_CONTINUE;
	}

	StartChangeScene();

	return UpdateStatus::UPDATE_CONTINUE;
}

//CleanUp current scene, change current scene (index), Start current Scene
bool ModuleScene::ChangeCurrentSceneRequest(uint index, int frames)
{
	changeTo = index;

	if (scenes[changeTo] == nullptr) return false;

	isChangingScene = true;

	changeState = SCENECHANGESTATES::fade_in;

	fadeSpeed = 10.0f;

	changeSceneWaitFrames = frames;

	return true;
}

bool ModuleScene::StartChangeScene()
{
	if (changeTo >= 0 && changeSceneRequest)
	{
		changeSceneRequest = false;

		if (scenes[changeTo] == nullptr) return false;

		scenes[currentScene]->CleanUp();

		app->renderer->ClearRederQueue();

		currentScene = (SCENES)changeTo;

		scenes[currentScene]->Start();

		fadeSpeed = -10.0f;

		changeTo = -1;

		changeState = SCENECHANGESTATES::fade_out;
	}

	return true;
}

void ModuleScene::ChangeSceneSteptoStep()
{
	fade += fadeSpeed;

	fade = fade > 255 ? 255 : fade < 0 ? 0 : fade;

	switch (changeState)
	{
	case SCENECHANGESTATES::idle:

		break;
	case SCENECHANGESTATES::fade_in:
		if (fade >= 255)
		{
			changeSceneRequest = true;
			changeState = SCENECHANGESTATES::idle;
		}
		break;
	case SCENECHANGESTATES::fade_out:
		if (fade <= 0)
		{
			isChangingScene = false;
			changeState = SCENECHANGESTATES::idle;
		}
		break;
	}
}

void ModuleScene::GetSaveData(pugi::xml_document& save)
{
	//currentScene->SetSaveData();

	//pugi::xml_node n = save.child("game_state").child("scene");

	//n.child("player").attribute("lifes") = playerSettings->playerLifes;
	//n.child("player").attribute("score") = playerSettings->playerScore;

	//n.child(currentScene->name.c_str()).child("player").attribute("x") = currentScene->playerX;
	//n.child(currentScene->name.c_str()).child("player").attribute("y") = currentScene->playerY;
}

void ModuleScene::LoadSaveData(pugi::xml_document& save)
{
	/*pugi::xml_node n = save.child("game_state").child("scene");

	currentScene->LoadSaveData(n);

	if (!playerSettings->isInit)
	{
		playerSettings->playerLifes = n.child("player").attribute("lifes").as_int();
		playerSettings->playerScore = n.child("player").attribute("score").as_int();
		playerSettings->isInit = true;
	}*/
}

void ModuleScene::ResetPlayerSettings()
{
	//playerSettings->Reset();
}

bool ModuleScene::CleanUp()
{
	for (int i = 0; i < SCENES_NUM; i++)
	{
		if (scenes[i] != nullptr)
		{
			scenes[i]->CleanUp();	//CleanUp all scenes (in case the Application is shut down)
			RELEASE(scenes[i]);
		}
	}

	return true;
}

/// <summary>
/// OBSOLETE
/// </summary>
void ModuleScene::DebugChangeScene()
{
	/*if (app->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
	{
		for (int i = 0; i < SCENES_NUM; i++)
		{
			if (app->input->GetKey(debugKeys[i]) == KEY_DOWN)
			{
				ChangeCurrentSceneRequest(i);
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		{
			ChangeCurrentSceneRequest(currentScene->getID());
		}
	}*/
}
