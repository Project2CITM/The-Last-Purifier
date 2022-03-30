#include <time.h>
#include "ModuleScene.h"
#include "ModuleAudio.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "TestScene.h"
#include "MainMenu.h"

ModuleScene::ModuleScene(bool start_enabled) : Module(start_enabled)
{
	name = "scenes";

	scenes[MAIN_MENU] = new MainMenu();
	scenes[LEVEL_1] = new TestScene();

	currentScene = MAIN_MENU;

	//scenes[0] = new SceneMainMenu(app);
	//scenes[1] = new SceneGameOver(app);
	//scenes[2] = new SceneLevel1(app, "level1");
	//scenes[3] = new SceneLevel2(app, "level2");
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

	//app->audio->PlayMusic("Assets/audio/music/pixelMusic.mp3", 2);

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

	if(isChangingScene)
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

	if (fade != 0) app->renderer->AddRectRenderQueue(SDL_Rect{ 0,0,(int)app->window->width,(int)app->window->height }, SDL_Color{ 0,0,0,255 }, true, 4, 200);

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
bool ModuleScene::ChangeCurrentSceneRequest(uint index)
{
	changeTo = index;

	isChangingScene = true;

	changeState = SCENECHANGESTATES::fade_in;

	if (scenes[changeTo] == nullptr) return false;

	fadeSpeed = 1.0f;

	return true;
}

bool ModuleScene::StartChangeScene()
{
	if (changeTo >= 0 && changeSceneRequest)
	{
		changeSceneRequest = false;

		//this->index = changeTo;

		if (scenes[changeTo] == nullptr) return false;

		scenes[currentScene]->CleanUp();

		app->renderer->ClearRederQueue();

		currentScene = (SCENES)changeTo;

		scenes[currentScene]->Start();

		fadeSpeed = -1.0f;

		changeTo = -1;

		changeState = SCENECHANGESTATES::fade_out;
	}

	return true;
}

void ModuleScene::ChangeSceneSteptoStep()
{
	fade += fadeSpeed;

	//if(fade >= 255)
		LOG("%i", changeState);

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
			delete scenes[i];
			scenes[i] = nullptr;
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