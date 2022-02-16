#ifndef __SCENE_H__
#define __SCENE_H__

#include "GameObject.h"
#include "List.h"
#include "GUI.h"

class Scene
{
protected:

	int ID = 0;

	int platformLenght = 2;

	Application* _app = nullptr;

	List<GameObject*> gameObjects;

	List<GUI*> guis;

	//List<SDL_Texture*> sceneTextures;

public:
	// Constructor
	Scene(Application* app, string name = "null");

	// Destructor
	~Scene();

	virtual bool InitScene();

	virtual bool Start();

	virtual bool PreUpdate();

	virtual bool Update();

	virtual bool PostUpdate();

	virtual bool CleanUp();

	void DestroyGameObject(GameObject* gameObject);

	virtual void SetSaveData();

	virtual void LoadSaveData(pugi::xml_node save);

	int getID()
	{
		return ID;
	}

	int lastID = 0;

	int isWin = false;

	std::string name = "null";
};

#endif //_SCENE_H_