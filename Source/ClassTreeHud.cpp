#include "ClassTreeHud.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "Scene.h"
#include <iostream>
#include "SceneGame.h"

ClassTreeHud::ClassTreeHud(PlayerClass pClass) : Scene("ClassTreeHud")
{
	this->pClass = pClass;
	Start();
}

ClassTreeHud::~ClassTreeHud()
{

}

bool ClassTreeHud::InitScene()
{
	Scene::InitScene();

	return true;
}

bool ClassTreeHud::Start()
{
	bPoint = { 10, 30 };

	bRect = { 10, 30, 215, 300 };

	tree = ClassTree::GetInstance();
	SceneGame* scene = (SceneGame*)Application::GetInstance()->scene->scenes[Application::GetInstance()->scene->currentScene];
	player = scene->player;

	switch (pClass)
	{
	case PlayerClass::REVENANT:
		treeTexture = app->textures->Load("Assets/Sprites/UI/Trees/Revenant_Tree.png");
		startId = REVENANT_FIRST_ID;
		break;
	case PlayerClass::SAGE:
		treeTexture = app->textures->Load("Assets/Sprites/UI/Trees/Sage_Tree.png");
		startId = SAGE_FIRST_ID;
		break;
	}

	//Buttons
	//testBtnPoint = {(bPoint.x + 125), (bPoint.y + 245)};

	unlockBtn = new List<GUIButton*>;

	iPoint aux = { 0, 0 };
	for (int i = startId; i < (startId+10); i++)
	{
		aux = tree->getSkillTree(i)->position;

		GUIButton* bttn = new GUIButton(aux, btnSize, btnSize, MenuButton::NONE, "Assets/Sprites/UI/Trees/Tree_Debug_Btn.png");
		bttn->setRenderColour({ 155, 0, 0, 155 });
		bttn->setRenderLayer(4, 1);

		unlockBtn->add(bttn);
	}

	//testBtn = new GUIButton(testBtnPoint, 40, 40, MenuButton::NONE);
	//testBtn->setRenderColour({ 155, 0, 0, 155 });
	//testBtn->setRenderLayer(4, 1);

	Scene::Start();

	return true;
}

bool ClassTreeHud::CleanUp()
{

	unlockBtn->clearPtr();
	delete unlockBtn;
	unlockBtn = nullptr;

	Scene::CleanUp();

	return true;
}

bool ClassTreeHud::PreUpdate()
{

	Scene::PreUpdate();

	return true;
}

bool ClassTreeHud::Update()
{
	//if (!active) return true;

	ListItem<GUIButton*>* element = unlockBtn->start;
	int aux = startId;
	while (element != NULL)
	{
		element->data->Update();

		if (element->data->doAction)
		{
			tree->unlockSkill(&player->souls, aux);
			element->data->doAction = false;
		}

		aux++;
		element = element->next;
	}

	return true;
}

bool ClassTreeHud::PostUpdate()
{
	//if (!active) return true;

	//Base
	app->renderer->AddRectRenderQueue(bRect, { 155, 0, 0, 255 }, true, 3, 1.0f, 0.0f);
	app->renderer->AddTextureRenderQueue(treeTexture, bPoint, { 0 , 0, 0, 0 }, 0.5f, 4, 0, 0, SDL_FLIP_NONE, 0.0f);

	//Buttons
	//testBtn->PostUpdate();

	ListItem<GUIButton*>* element = unlockBtn->start;

	while (element != NULL)
	{
		element->data->PostUpdate();

		element = element->next;
	}

	return true;
}