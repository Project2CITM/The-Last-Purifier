#include "Scene.h"
#include "ModuleRender.h"
#include "ModuleEvents.h"

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
	if (app->isPause) return true;

	for (int i = 0; i < gameObjects.count(); i++)
	{
		if (!gameObjects[i]) continue;

		if (gameObjects[i]->pendingToDelete)
		{
			DestroyGameObject(gameObjects[i]);
		}

		else if (gameObjects[i]->enable)gameObjects[i]->PreUpdate();
	}

	for (int i = 0; i < texts.count(); i++)
	{
		if (!texts[i]) continue;

		if (texts[i]->pendingToDelate) DestroyText(texts[i]);

		else texts[i]->PreUpdate();
	}

	return true;
}

bool Scene::Update()
{
	if (app->isPause) return true;

	for (int i = 0; i < gameObjects.count(); i++)
	{
		if (gameObjects[i] && gameObjects[i]->enable) gameObjects[i]->Update();
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
		if (gameObjects[i] && gameObjects[i]->enable) gameObjects[i]->PostUpdate();
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
	guisOptions.clearPtr();
	guisCredtis.clearPtr();
	guisPause.clearPtr();
	guisSettingsP.clearPtr();
	guisControls.clearPtr();

	texts.clearPtr();

	if (app->renderer->camera != nullptr)
	{
		app->renderer->camera->ReleaseTarget();
	}

	app->events->TriggerEvent(GameEvent::DELETING_SCENE);

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

void Scene::AddGUIOptions(GUI* gui)
{
	guisOptions.add(gui);
}

void Scene::AddGUICredtis(GUI* gui)
{
	guisCredtis.add(gui);
}

void Scene::AddGUIPause(GUI* gui)
{
	guisPause.add(gui);
}

void Scene::AddGUISettingsP(GUI* gui)
{
	guisSettingsP.add(gui);
}

void Scene::AddGUIControls(GUI* gui)
{
	guisControls.add(gui);
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
		gameObject->CleanUp();
		gameObjects.delPtr(gameObjects.At(index));
	}
}

void Scene::DestroyGUI(GUI* gui)
{
	int indexMainMenu = guisMainMenu.find(gui);
	int indexOptions = guisOptions.find(gui);
	int indexCredtis = guisCredtis.find(gui);
	int indexPause = guisPause.find(gui);
	int indexControls = guisControls.find(gui);
	int indexSettingsP = guisSettingsP.find(gui);

	if (indexMainMenu >= 0)
	{
		guisMainMenu.delPtr(guisMainMenu.At(indexMainMenu));
	}

	if (indexOptions >= 0)
	{
		guisOptions.delPtr(guisOptions.At(indexOptions));
	}

	if (indexCredtis >= 0)
	{
		guisCredtis.delPtr(guisCredtis.At(indexCredtis));
	}

	if (indexPause >= 0)
	{
		guisPause.delPtr(guisPause.At(indexPause));
	}

	if (indexControls >= 0)
	{
		guisControls.delPtr(guisControls.At(indexControls));
	}

	if (indexSettingsP >= 0)
	{
		guisSettingsP.delPtr(guisSettingsP.At(indexSettingsP));
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