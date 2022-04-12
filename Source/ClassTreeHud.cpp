#include "ClassTreeHud.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include <iostream>

ClassTreeHud::ClassTreeHud() : Scene("ClassTreeHud")
{

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

	treeTexture = app->textures->Load("Assets/Sprites/UI/Trees/Revenant_Tree.png");

	//Buttons
	//testBtnPoint = {(bPoint.x + 125), (bPoint.y + 245)};

	iPoint aux = { 0, 0 };
	//for (int i = 0; i < TREE_SIZE; i++)
	//{
	//	//aux = {app->scene.}

	//	unlockBtn[i] = new GUIButton(testBtnPoint, btnSize, btnSize, MenuButton::NONE);
	//	unlockBtn[i]->setRenderColour({ 155, 0, 0, 155 });
	//	unlockBtn[i]->setRenderLayer(4, 1);
	//}

	//testBtn = new GUIButton(testBtnPoint, 40, 40, MenuButton::NONE);
	//testBtn->setRenderColour({ 155, 0, 0, 155 });
	//testBtn->setRenderLayer(4, 1);

	Scene::Start();

	return true;
}

bool ClassTreeHud::CleanUp()
{
	//testBtn = nullptr;

	Scene::CleanUp();

	return true;
}

bool ClassTreeHud::PreUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		active = !active;
	}

	Scene::PreUpdate();

	return true;
}

bool ClassTreeHud::Update()
{
	if (!active) return true;



	return true;
}

bool ClassTreeHud::PostUpdate()
{
	if (!active) return true;

	//

	app->renderer->AddRectRenderQueue(bRect, { 155, 0, 0, 255 }, true, 3, 1.0f, 0.0f);
	app->renderer->AddTextureRenderQueue(treeTexture, bPoint, { 0 , 0, 0, 0 }, 0.5f, 4, 0, 0, SDL_FLIP_NONE, 0.0f);

	//Buttons
	//testBtn->PostUpdate();

	return true;
}