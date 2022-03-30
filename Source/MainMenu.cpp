#include "MainMenu.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleScene.h"
#include "GUIButton.h"

// Si no lo pone aqui, sale un menoryleak, nose porque
RenderObject fondo;

MainMenu::MainMenu():Scene("MainMenu")
{

}

MainMenu::~MainMenu()
{
}

bool MainMenu::InitScene()
{

	Scene::InitScene();

	return true;
}

bool MainMenu::Start()
{
	fondo.InitAsTexture(app->textures->Load("Assets/Sprites/UI/MainMenu/mainmenu.png"), { 0,0 }, {0,0,0,0}, 0.5f);

	PlayBUT = new GUIButton({ 0, 0 }, 100, 100);

	Scene::Start();

	return true;
}

bool MainMenu::PreUpdate()
{
	Scene::PreUpdate();

	return true;
}

bool MainMenu::Update()
{
	//if (PlayBUT->navigation)
	//{
	//	
	//}

	if (PlayBUT->doAction)
	{
		app->scene->ChangeCurrentSceneRequest(LEVEL_1);
	}
	Scene::Update();

	return true;
}

bool MainMenu::PostUpdate()
{
	app->renderer->AddRenderObjectRenderQueue(fondo);
	Scene::PostUpdate();

	return true;
}

bool MainMenu::CleanUp()
{
	Scene::CleanUp();

	return true;
}