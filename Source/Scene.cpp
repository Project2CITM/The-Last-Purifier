#include "Scene.h"

Scene::Scene(Application* app, string name)
{
	_app = app;
	this->name = name;
}

Scene::~Scene()
{
}

bool Scene::InitScene()
{
	/*
	// Obstacles
	for (int i = 0; i < _app->map->mapObjects.count(); i++)
	{
		if (_app->map->mapObjects[i].id == 0)
		{
			GameObject* g = new GameObject("wall", "Wall", _app);
			// +8 = offset, porque pivot de b2Body es el centro, y de tectura es izquierda superiol.
			g->pBody = _app->physics->CreateRectangle({ _app->map->mapObjects[i].position.x + 8, _app->map->mapObjects[i].position.y + 8 }, 16, 16, g);
			g->pBody->body->SetType(b2BodyType::b2_staticBody);
			g->pBody->body->GetFixtureList()->SetFriction(0);
			g->adjustToGrid = true;
			gameObjects.add(g);
		}
		else if (_app->map->mapObjects[i].id == 1)
		{
			Platform* g = new Platform({ _app->map->mapObjects[i].position.x , _app->map->mapObjects[i].position.y }, "platform", "Platform", _app, platformLenght);
			g->adjustToGrid = true;
			gameObjects.add(g);
		}
		else if (_app->map->mapObjects[i].id == 2)
		{
			Saw* saw = new Saw({ _app->map->mapObjects[i].position.x ,_app->map->mapObjects[i].position.y }, "saw", "Saw", _app);
			gameObjects.add(saw);
		}
		else if (_app->map->mapObjects[i].id == 3)
		{
			// +8 = offset (depende del tile, no de mapa), porque pivot de b2Body es el centro, y de tectura es izquierda superiol.
			Spike* spike = new Spike({ _app->map->mapObjects[i].position.x + 8 ,_app->map->mapObjects[i].position.y + 8 }, _app->map->mapObjects[i].rotation, "spike", "Spike", _app);
			gameObjects.add(spike);
		}
		else if (_app->map->mapObjects[i].id == 4)
		{
			FireTrap* ft = new FireTrap({ _app->map->mapObjects[i].position.x + 8 ,_app->map->mapObjects[i].position.y }, "fireTrap2", "FireTrap", _app);
			gameObjects.add(ft);
		}
		else if (_app->map->mapObjects[i].id == 5)
		{
			//CheckPoint* chekPoint = new CheckPoint({ _app->map->mapObjects[i].position.x ,_app->map->mapObjects[i].position.y + 4}, "checkpoint", "Checkpoint", _app);
			//gameObjects.add(chekPoint);
		}
	}

	// Envirouments
	pugi::xml_node enviroument = _app->scene->config.child(name.c_str()).child("enviroument");

	for (enviroument = enviroument.first_child(); enviroument; enviroument = enviroument.next_sibling())
	{
		string name = enviroument.name();
		if (name == "backGround")
		{
			BackGround* bg = new BackGround(enviroument.attribute("name").as_string("null"), enviroument.attribute("tag").as_string("null"), _app);

			gameObjects.add(bg);
		}
		if (name == "winTrigger")
		{
			iPoint position = { enviroument.attribute("positionX").as_int(0),enviroument.attribute("positionY").as_int(0) };

			GameObject* winTrigger = new GameObject("winTrigger", "WinTrigger", _app);

			winTrigger->pBody = _app->physics->CreateRectangleSensor(position, enviroument.attribute("width").as_int(0), enviroument.attribute("height").as_int(0), winTrigger);

			gameObjects.add(winTrigger);
		}
		if (name == "mobilePlatform")
		{
			iPoint position = { enviroument.attribute("positionX").as_int(0),enviroument.attribute("positionY").as_int(0) };

			iPoint moveDistance = { enviroument.attribute("moveX").as_int(0),enviroument.attribute("moveY").as_int(0) };

			MobilePlatform* mobPlatform = new MobilePlatform
			(position, "mobilePlatform", enviroument.attribute("tag").as_string("MobilePlatform"), _app, enviroument.attribute("lenght").as_int(1), moveDistance,
				enviroument.attribute("moveSpeed").as_int(0), enviroument.attribute("loop").as_bool(true), enviroument.attribute("stopTime").as_int(0));
				
			gameObjects.add(mobPlatform);
		}
	}
	*/

	return true;
}

bool Scene::Start()
{
	return true;
}

bool Scene::PreUpdate()
{
	return true;
}

bool Scene::Update()
{
	for (int i = 0; i < guis.count(); i++)
	{
		if (guis[i]) guis[i]->Update();
	}
	return true;
}

bool Scene::PostUpdate()
{
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