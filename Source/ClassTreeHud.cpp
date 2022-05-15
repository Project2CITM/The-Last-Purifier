#include "ClassTreeHud.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "Scene.h"
#include <iostream>
#include "SceneGame.h"
#include "AssetsManager.h"

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
	char* buffer = 0;
	pugi::xml_document dataFile;

	int bytesFile = app->assetManager->LoadData("data.xml", &buffer);

	pugi::xml_parse_result result = dataFile.load_buffer(buffer, bytesFile);

	RELEASE_ARRAY(buffer);

	bPoint = { 40, 13 };

	bRect = { 40, 30, 563, 304 };

	tree = ClassTree::GetInstance();
	cTree = CommonTree::GetInstance();
	SceneGame* scene = (SceneGame*)Application::GetInstance()->scene->scenes[Application::GetInstance()->scene->currentScene];
	player = scene->player;

	switch (pClass)
	{
	case PlayerClass::REVENANT:
		treeTexture = app->textures->Load("Sprites/UI/Trees/Revenant_Tree.png");
		startId = REVENANT_FIRST_ID;
		break;
	case PlayerClass::SAGE:
		treeTexture = app->textures->Load("Sprites/UI/Trees/Sage_Tree.png");
		startId = SAGE_FIRST_ID;
		break;
	}

	commonTreeTexture = app->textures->Load("Sprites/UI/Trees/Common_Tree.png");

	feedbackTex = app->textures->Load("Sprites/UI/Trees/SpellIcons/SpellIcons.png");
	cmmFeedbackTex = app->textures->Load("Sprites/UI/Trees/SpellIcons/UpgreadesIcons.png");

	//Buttons
	cmmUnlockBtn = new List<GUIButton*>;
	cmmFeed = new List<treeFeedback*>;
	unlockBtn = new List<GUIButton*>;
	feed = new List<treeFeedback*>;


	treeSwitch = new GUIButton({ 20, 160 }, 46, 46, MenuButton::NONE, app->textures->Load("Sprites/UI/Trees/Switch.png"));
	treeSwitch->setRenderColour({ 155, 0, 0, 155 });
	treeSwitch->setRenderLayer(4, 1);
	iPoint aux = { 0, 0 };

	ListItem<TreeElement*>* element = cTree->treeList->start;
	while (element != NULL)
	{
		aux = element->data->position;

		GUIButton* bttn = nullptr;
		if (element->data->type == CommonUpgrades::SKILL_SLOT || element->data->type == CommonUpgrades::DECK || element->data->type == CommonUpgrades::LUCK)
		{
			bttn = new GUIButton(aux, 30, 32, MenuButton::NONE, app->textures->Load("Sprites/UI/Trees/Big_Common_Tree_Debug_Btn.png"));
			aux = { aux.x + 2, aux.y + 11 };
		}
		else
		{
			bttn = new GUIButton(aux, 30, 14, MenuButton::NONE, app->textures->Load("Sprites/UI/Trees/Common_Tree_Debug_Btn.png"));
			aux = { aux.x + 2, aux.y + 2 };
		}
		bttn->setRenderColour({ 155, 0, 0, 155 });
		bttn->setRenderLayer(4, 1);

		cmmUnlockBtn->add(bttn);

		cmmFeed->add(new treeFeedback(element->data->type, element->data->unlocked, aux, element->data->id));

		element = element->next;
	}

	
	for (int i = startId; i < (startId + MAX_SPELLL_BY_CLASS); i++)
	{
		aux = tree->getSkillTree(i)->position;

		//Button
		GUIButton* bttn = new GUIButton(aux, btnSize, btnSize, MenuButton::NONE, app->textures->Load("Sprites/UI/Trees/Tree_Debug_Btn.png"));
		bttn->setRenderColour({ 155, 0, 0, 155 });
		bttn->setRenderLayer(4, 1);

		unlockBtn->add(bttn);

		//Feedback
		feed->add(new treeFeedback(i, tree->getCurrentLevel(i), aux));
	}

	Scene::Start();

	return true;
}

bool ClassTreeHud::CleanUp()
{

	unlockBtn->clearPtr();
	unlockBtn->clear();
	delete unlockBtn;
	unlockBtn = nullptr;

	feed->clearPtr();
	feed->clear();
	delete feed;
	feed = nullptr;

	cmmUnlockBtn->clearPtr();
	cmmUnlockBtn->clear();
	delete cmmUnlockBtn;
	cmmUnlockBtn = nullptr;

	cmmFeed->clearPtr();
	cmmFeed->clear();
	delete cmmFeed;
	cmmFeed = nullptr;

	delete treeSwitch;
	treeSwitch = nullptr;

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
	//Tree Switch
	treeSwitch->Update();
	if (treeSwitch->doAction)
	{
		switcher = !switcher;
		treeSwitch->doAction = false;
	}

	//if (!active) return true;
	ListItem<GUIButton*>* element = nullptr;
	ListItem<treeFeedback*>* element2 = nullptr;

	if (switcher)
	{ //Class tree logic
		element = unlockBtn->start;
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

		element2 = feed->start;
		aux = startId;
		while (element2 != NULL)
		{
			element2->data->currentFrame = tree->getCurrentLevel(aux);

			aux++;
			element2 = element2->next;
		}
	}
	else
	{ //Common tree logic
		element = cmmUnlockBtn->start;
		int aux = -1;
		int upInt = -1;
		while (element != NULL)
		{
			element->data->Update();
			aux++;

			if (element->data->doAction)
			{
				upInt = aux;
				element->data->doAction = false;
			}
			
			element = element->next;
		}

		element2 = cmmFeed->start;

		while (element2 != NULL)
		{
			if (element2->data->id == upInt)
			{
				cTree->Upgrade(&player->souls, upInt);
			}
			
			element2->data->currentFrame = cTree->getElement(element2->data->id)->unlocked;

			element2 = element2->next;
		}

	}


	return true;
}

bool ClassTreeHud::PostUpdate()
{
	//Tree Switch
	treeSwitch->PostUpdate();

	ListItem<GUIButton*>* element = nullptr;
	ListItem<treeFeedback*>* element2 = nullptr;

	if (switcher)
	{
		app->renderer->AddTextureRenderQueue(treeTexture, bPoint, { 0 , 0, 0, 0 }, 0.55f, 4, 0, 0, SDL_FLIP_NONE, 0.0f);
		element = unlockBtn->start;
		element2 = feed->start;
	}
	else
	{
		app->renderer->AddTextureRenderQueue(commonTreeTexture, bPoint, { 0 , 0, 0, 0 }, 0.55f, 4, 0, 0, SDL_FLIP_NONE, 0.0f);
		element = cmmUnlockBtn->start;
		element2 = cmmFeed->start;
	}


	while (element != NULL)
	{
		element->data->PostUpdate();

		element = element->next;
	}



	while (element2 != NULL)
	{
		if (switcher)
			app->renderer->AddTextureRenderQueue(feedbackTex, element2->data->pos, element2->data->anim.getFrame(element2->data->currentFrame), 1.10f, 4, 2, 0, SDL_FLIP_NONE, 0.0f);
		else
			app->renderer->AddTextureRenderQueue(cmmFeedbackTex, element2->data->pos, element2->data->anim.getFrame(element2->data->currentFrame), 0.55f, 4, 2, 0, SDL_FLIP_NONE, 0.0f);

		element2 = element2->next;
	}



	return true;
}