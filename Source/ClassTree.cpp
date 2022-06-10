#include "ClassTree.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleScene.h"
#include "SceneGame.h"

ClassTree* ClassTree::instance = nullptr;

ClassTree::ClassTree(PlayerClass pClass)
{
	this->listenTo[0] = GameEvent::SAVE_GAME;

	playerClass = pClass;
	Application::GetInstance()->events->AddListener(this);

	Start();
}

ClassTree::~ClassTree()
{
	
}

ClassTree* ClassTree::GetInstance()
{
	if (instance == nullptr)
	{
		SceneGame* scene = (SceneGame*)Application::GetInstance()->scene->scenes[Application::GetInstance()->scene->currentScene];
		instance = new ClassTree(scene->player->playerClass);
	}
	return instance;
}

void ClassTree::Start()
{
	pugi::xml_parse_result result1;
	pugi::xml_parse_result result2;

	result2 = saveFile.load_file(SAVE_CLASS_TREE_XML);

	switch (playerClass)
	{
	case PlayerClass::REVENANT:
		result1 = classFile.load_file(REVENANT_TREE_XML);
		break;
	case PlayerClass::SAGE:
		result1 = classFile.load_file(SAGE_TREE_XML);
		break;
	}

	if (result1 == NULL) LOG("Could not load xml file: %s. pugi error: %s", REVENANT_TREE_XML, result1.description());
	if (result2 == NULL) LOG("Could not load xml file: %s. pugi error: %s", SAVE_CLASS_TREE_XML, result2.description());

	LoadBaseTree();
	SaveLoadTree(true);
}

void ClassTree::PreUpdate()
{

}

void ClassTree::Update()
{
	//std::cout << "A Skill name ->" << skillTree[3]->name << std::endl;

	//if (active)
	//{	//Loads Tree elements 
		//std::cout << "Git gud!" << std::endl;
	//}
}

void ClassTree::PostUpdate()
{

	//if (active)
	//{	//Draws the ClassTree

	//	app->renderer->AddRectRenderQueue(rect, { 155, 0, 0, 255 }, true, 3, 1.0f, 0.0f);
	//	app->renderer->AddTextureRenderQueue(treeTexture, drawP, {0 , 0, 0, 0}, 0.5f, 4, 0, 0, SDL_FLIP_NONE, 1.0f);
	//	std::cout << "x->" << drawP.x << " |y->" << drawP.y << std::endl;
	//}
}

void ClassTree::ReleaseInstance()
{
	if (instance != nullptr)
	{
		ClassTree::CleanUp();
		delete instance;
		instance = nullptr;
	}
}

void ClassTree::CleanUp()
{
	//RELEASE(classFile);
	
	Application::GetInstance()->events->RemoveListener(instance);

	for (int i = 0; i < TREE_SIZE; i++)
	{
		RELEASE(this->instance->skillTree[i]);
	}
}

bool ClassTree::unlockSkill(int* classPoints, int skillId)
{
	SkillTreeElement* theSkill = getSkillTree(skillId);
	if (theSkill == nullptr) return false;

	SkillTreeElement* theRequiredSkill = getSkillTree(theSkill->requiresID);
	if (theRequiredSkill == nullptr && (skillId == 1 || skillId == 11))
	{
		//Checks if the skill with the given id is already maxed(level)
		if (theSkill->currentLevel < theSkill->maxLevel)
		{
			//Checks if there are enough points to unlock
			if (*classPoints >= theSkill->cost)
			{   //Unlocks
				theSkill->currentLevel = (SkillLevel)((int)theSkill->currentLevel + 1);
				*classPoints -= theSkill->cost;

				return true;
			}
		}
		return false;
	}
	else if (theRequiredSkill == nullptr) return false;
	
	//Checks if the skill with the given id is already maxed(level)
	if (theSkill->currentLevel < theSkill->maxLevel)
	{
		//Checks if the required skill is already unlocked
		if (theRequiredSkill->currentLevel >= SkillLevel::BLUE)
		{
			//Checks if there are enough points to unlock
			if (*classPoints >= theSkill->cost)
			{   //Unlocks
				theSkill->currentLevel = (SkillLevel) ((int)theSkill->currentLevel + 1);
				*classPoints -= theSkill->cost;
				
				return true;
			}
		}
		return false;
	}

	//Return false means that there is nothing to unlock.
	return false;
}

bool ClassTree::LoadBaseTree()
{
	pugi::xml_node bNode = classFile.first_child().child("class_tree");

	/*while (bNode.attribute("id").as_int() != (int) playerClass)
	{
		bNode = bNode.next_sibling();
	}*/
	bNode = bNode.first_child();
	
	for (int i = 0; i < TREE_SIZE; i++)
	{
		iPoint temp = { bNode.child("position").attribute("x").as_int(), bNode.child("position").attribute("y").as_int() };
		skillTree[i] = new SkillTreeElement(
			bNode.attribute("id").as_int(),
			(std::string)bNode.child_value("name"),
			(std::string)bNode.child_value("description"),
			bNode.child("cost").text().as_int(),
			bNode.child("requiresID").text().as_int(),
			temp,
			(SkillLevel) bNode.child("maxLevel").text().as_int()
		);

		bNode = bNode.next_sibling();
	}

	return true;
}

bool ClassTree::SaveLoadTree(bool load)
{
	if (saveFile == nullptr) return false;

	pugi::xml_node bNode = saveFile.first_child().child("class_tree");

	/*while (bNode.attribute("id").as_int() != (int)playerClass)
	{
		bNode = bNode.next_sibling();
	}*/
	bNode = bNode.first_child();

	for (int i = 0; i < TREE_SIZE; i++)
	{
		if (load) //Loads
		{
			skillTree[i]->currentLevel = (SkillLevel)bNode.attribute("level").as_int();
		}
		else //Saves
		{
			bNode.attribute("level") = (int)skillTree[i]->currentLevel;
		}

		bNode = bNode.next_sibling();
	};

	if (!load) //In case is called as a Save, it saves it back to the file
	{
		saveFile.save_file(SAVE_CLASS_TREE_XML);
	}


	return true;
}

void ClassTree::GameEventTriggered(GameEvent id)
{
	SaveLoadTree();
}

/*
  GETTERS AND SETTERS
*/

SkillTreeElement* ClassTree::getSkillTree(int value)
{
	//value--; //Equals the value of the enum id to the array position (Enum starts at 1 because 0 = NONE)
	//if (value >= TREE_SIZE) return skillTree[TREE_SIZE - 1];
	return skillTree[value-1];
}

/// <summary>
/// Returns Level 3 and 4 as (int) 1 and 2. Returns other levels as their normal int value.
/// </summary>
/// <param name="id"></param>
/// <returns></returns>
int ClassTree::getCurrentLevel(int id) 
{
	SkillTreeElement* theSkill = getSkillTree(id);

	int toReturn = 0;
	if (theSkill == nullptr) return toReturn;

	if (theSkill->currentLevel == SkillLevel::PURPLE && theSkill->maxLevel == SkillLevel::YELLOW) toReturn = 1;
	else if (theSkill->currentLevel == SkillLevel::YELLOW) toReturn = 2;
	else toReturn = (int)theSkill->currentLevel;

	return toReturn;
}


std::string ClassTree::getDescription(int id)
{
	return getSkillTree(id)->description;
}