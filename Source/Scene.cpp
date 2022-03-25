#include "Scene.h"
#include <string.h>

Scene::Scene(std::string name)
{
	app = Application::GetInstance();
	this->name = name;
}

Scene::~Scene()
{
}

bool Scene::InitScene()
{

	return true;
}

bool Scene::Start()
{
	for (int i = 0; i < gameObjects.count(); i++)
	{
		if (gameObjects[i])
		{
			gameObjects[i]->Start();
		}
	}

	return true;
}

bool Scene::PreUpdate()
{
	for (int i = 0; i < gameObjects.count(); i++)
	{
		if (gameObjects[i])
		{
			if (gameObjects[i]->pendingToDelete) DestroyGameObject(gameObjects[i]);
			else gameObjects[i]->PreUpdate();
		}
	}

	return true;
}

bool Scene::Update()
{
	for (int i = 0; i < gameObjects.count(); i++)
	{
		if (gameObjects[i])
		{
			gameObjects[i]->Update();
		}
	}

	for (int i = 0; i < guis.count(); i++)
	{
		if (guis[i]) guis[i]->Update();
	}
	return true;
}

bool Scene::PostUpdate()
{
	for (int i = 0; i < gameObjects.count(); i++)
	{
		if (gameObjects[i])
		{
			gameObjects[i]->PostUpdate();
		}
	}

	for (int i = 0; i < guis.count(); i++)
	{
		if (guis[i]) guis[i]->PostUpdate();
	}
	return true;
}

bool Scene::CleanUp()
{
	for (int i = 0; i < gameObjects.count(); i++)
	{
		if (gameObjects[i])
		{		
			gameObjects[i]->CleanUp();
		}
	}

	gameObjects.clearPtr();

	guis.clearPtr();

	return true;
}

void Scene::AddGameObject(GameObject* gameObject)
{
	gameObjects.add(gameObject);
}

void Scene::DestroyGameObject(GameObject* gameObject)
{
	int index = gameObjects.find(gameObject);

	if (index >= 0)
	{
		gameObjects.delPtr(gameObjects.At(index));
	}
}

void Scene::SetSaveData()
{
}

void Scene::LoadSaveData(pugi::xml_node save)
{
}