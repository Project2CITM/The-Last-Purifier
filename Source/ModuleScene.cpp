#include <time.h>
#include "ModuleScene.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "TestScene.h"
#include "ModuleWindow.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "scenes";

	scenes[0] = new TestScene(app);

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
	currentScene = scenes[0];

	bool ret = true;

	if (currentScene == nullptr)
	{
		return ret;
	}

	currentScene->Start();
	
	//App->audio->PlayMusic("Assets/audio/music/pixelMusic.mp3", 2);

	//Mix_VolumeMusic(App->saveF.child("game_state").child("settings").attribute("music").as_float() * 60);

	//fadePanel = App->textures->Load("Assets/textures/Menu/fadePanel.png");

	return ret;
}

UpdateStatus ModuleScene::PreUpdate()
{
	if (currentScene == nullptr)
	{
		return UpdateStatus::UPDATE_CONTINUE;
	}

	currentScene->PreUpdate();

	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleScene::Update()
{
	if (currentScene == nullptr)
	{
		return UpdateStatus::UPDATE_CONTINUE;
	}

	if(isChangingScene)
	{
		ChangeSceneSteptoStep();

		return UpdateStatus::UPDATE_CONTINUE;
	}

	if (!currentScene->Update())
	{
		return UpdateStatus::UPDATE_STOP;
	}

	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleScene::PostUpdate()
{
	if (currentScene == nullptr)
	{
		return UpdateStatus::UPDATE_CONTINUE;
	}

	currentScene->PostUpdate();

	if (fade != 0) App->renderer->AddRectRenderQueue(SDL_Rect{ 0,0,(int)App->window->width,(int)App->window->height }, SDL_Color{ 0,0,0,255 }, 4, 200);

	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleScene::EndUpdate()
{
	if (currentScene == nullptr)
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

	if (scenes[changeTo] == nullptr) return false;

	fadeSpeed = 1.0f;

	return true;
}

bool ModuleScene::StartChangeScene()
{
	if (changeTo >= 0 && changeSceneRequest)
	{
		changeSceneRequest = false;

		currentSceneState = (SCENES)changeTo;

		this->index = changeTo;

		if (scenes[changeTo] == nullptr) return false;

		currentScene->CleanUp();

		App->renderer->ClearRederQueue();

		currentScene = scenes[changeTo];

		currentScene->Start();

		fadeSpeed = -1.0f;

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
	/*if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
	{
		for (int i = 0; i < SCENES_NUM; i++)
		{
			if (App->input->GetKey(debugKeys[i]) == KEY_DOWN)
			{
				ChangeCurrentSceneRequest(i);
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		{
			ChangeCurrentSceneRequest(currentScene->getID());
		}
	}*/
}