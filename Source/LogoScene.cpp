#include "LogoScene.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleScene.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "AssetsManager.h"

RenderObject Logos[2];

LogoScene::LogoScene():Scene("logoScene")
{
}

LogoScene::~LogoScene()
{
}

bool LogoScene::InitScene()
{
	Scene::InitScene();

	return true;
}

bool LogoScene::Start()
{
	char* buffer = 0;
	pugi::xml_document dataFile;

	int bytesFile = app->assetManager->LoadData("data.xml", &buffer);

	pugi::xml_parse_result result = dataFile.load_buffer(buffer, bytesFile);

	RELEASE_ARRAY(buffer);

	Logos[0].InitAsTexture(app->textures->Load("Sprites/UI/Team_Logo.png"), { 170,40 }, { 0,0,0,0 }, 1);
	Logos[1].InitAsTexture(app->textures->Load("Sprites/UI/logoGame.png"), { 170,40 }, { 0,0,0,0 }, 1);
	
	LogoAppear_FX = app->audio->LoadFx("Audio/SFX/UI/logoFX.wav");
	app->audio->PlayFx(LogoAppear_FX);

	Scene::Start();

	return true;
}

bool LogoScene::PreUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN ||app->input->GetControllerButton(JoystickButtons::BUTTON_A) == KEY_DOWN)
	{
		count = 456;
	}

	Scene::PreUpdate();

	return true;
}

bool LogoScene::Update()
{
	if (count > 200) drawLogo = 1;
	if (count > 455) app->scene->ChangeCurrentSceneRequest(MAIN_MENU);
	else
	{
		GaussEffect();
		count++;
	}

	Scene::Update();

	return true;
}

bool LogoScene::PostUpdate()
{
	app->renderer->AddRenderObjectRenderQueue(Logos[drawLogo]);

	Scene::PostUpdate();

	return true;
}

bool LogoScene::CleanUp()
{
	Scene::CleanUp();

	return true;
}

void LogoScene::GaussEffect()
{
	gaussCount--;

	if (gaussCount <= 0)
	{
		gaussCount = 5;

		app->textures->SetGauss(Logos[0].texture, gaussPower);

		gaussPower -= 0.1f;

		if (gaussPower < 0.43f) gaussPower = 0.43f;
	}
}