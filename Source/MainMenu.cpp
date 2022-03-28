#include "MainMenu.h"
#include <string.h>

MainMenu::MainMenu(std::string name)
{
	app = Application::GetInstance();
	this->name = name;
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
	Scene::Update();

	return true;
}

bool MainMenu::PostUpdate()
{
	Scene::PostUpdate();

	return true;
}

bool MainMenu::CleanUp()
{
	Scene::CleanUp();

	return true;
}

void MainMenu::SetSaveData()
{

}

void MainMenu::LoadSaveData(pugi::xml_node save)
{

}