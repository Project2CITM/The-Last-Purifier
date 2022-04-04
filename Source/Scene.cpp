#include "Scene.h"

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

	for (int i = 0; i < texts.count(); i++)
	{
		if (texts[i])
		{
			if (texts[i]->pendingToDelate) DestroyText(texts[i]);
			else texts[i]->PreUpdate();
		}
	}

	return true;
}

bool Scene::Update()
{
	for (int i = 0; i < gameObjects.count(); i++)
	{
		if (gameObjects[i]) gameObjects[i]->Update();
	}



	for (int i = 0; i < texts.count(); i++)
	{
		if (texts[i]) texts[i]->Update();
	}

	return true;
}

bool Scene::PostUpdate()
{
	for (int i = 0; i < gameObjects.count(); i++)
	{
		if (gameObjects[i]) gameObjects[i]->PostUpdate();
	}

	

	for (int i = 0; i < texts.count(); i++)
	{
		if (texts[i]) texts[i]->PostUpdate();
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

	guisMainMenu.clearPtr();
	guisCredtis.clearPtr();

	texts.clearPtr();

	return true;
}

void Scene::AddGameObject(GameObject* gameObject)
{
	gameObjects.add(gameObject);
}

void Scene::AddGUIMainMenu(GUI* gui)
{
	guisMainMenu.add(gui);
}

void Scene::AddGUICredtis(GUI* gui)
{
	guisCredtis.add(gui);
}

void Scene::AddText(Text* text)
{
	texts.add(text);
}

void Scene::DestroyGameObject(GameObject* gameObject)
{
	int index = gameObjects.find(gameObject);

	if (index >= 0)
	{
		gameObjects.delPtr(gameObjects.At(index));
	}
}

void Scene::DestroyGUI(GUI* gui)
{
	int indexMainMenu = guisMainMenu.find(gui);
	int indexCredtis = guisCredtis.find(gui);

	if (indexMainMenu >= 0)
	{
		guisMainMenu.delPtr(guisMainMenu.At(indexMainMenu));
	}
	if (indexCredtis >= 0)
	{
		guisCredtis.delPtr(guisCredtis.At(indexCredtis));

	}
}

void Scene::DestroyText(Text* text)
{
	int index = texts.find(text);

	if (index >= 0)
	{
		texts.delPtr(texts.At(index));
	}
}

void Scene::SetSaveData()
{
}

void Scene::LoadSaveData(pugi::xml_node save)
{
}