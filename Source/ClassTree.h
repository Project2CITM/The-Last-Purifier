#ifndef __CLASS_TREE_H__
#define __CLASS_TREE_H__
#include <string>

#include "GameObject.h"
#include "Globals.h"
#include "Point.h"
#include "Player.h"
#include "External/PugiXml/src/pugixml.hpp"
#include "ModuleEvents.h"

#define TREE_SIZE 20
#define SAGE_TREE_XML "BaseClassTree.xml"
#define REVENANT_TREE_XML "BaseClassTree.xml"
#define SAVE_CLASS_TREE_XML "SaveClassTree.xml"

enum class SkillLevel {
	LOCKED = 0,
	GREEN,
	BLUE,
	PURPLE,
	YELLOW
};

enum InitialClassId
{
	REVENANT_FIRST_ID = 1,
	SAGE_FIRST_ID = 11
};

struct SkillTreeElement
{
	int id = 0;
	std::string name = "";
	std::string description = "";
	int cost = 100;

	int requiresID = 0;

	SkillLevel currentLevel = SkillLevel::LOCKED;
	SkillLevel maxLevel = SkillLevel::PURPLE;

	iPoint position;

	SkillTreeElement(int id, std::string name, std::string description, int cost, int requiresID, iPoint position, SkillLevel maxLevel = SkillLevel::PURPLE)
	{
		this->id = id;
		this->name = name;
		this->description = description;
		this->cost = cost;
		this->requiresID = requiresID;
		this->position = position;

		this->maxLevel = maxLevel;
	};
};

class ClassTree : public EventListener
{
public:

	static ClassTree* GetInstance();

	void Start();

	void PreUpdate();

	void Update();

	void PostUpdate();

	void ReleaseInstance();

	void CleanUp();

	void GameEventTriggered() override;

public:
	SkillTreeElement* getSkillTree(int value);
	bool unlockSkill(int* classPoints, int skillId);
	bool levelUpCheck(SkillTreeElement* theSkill);

	//Loads the base tree
	bool LoadBaseTree();

	//Saves and loads the tree (in case there is a save file)
	bool SaveLoadTree(bool load = false);


	int getCurrentLevel(int id);

private:

	static ClassTree* instance;

	ClassTree(PlayerClass pClass);
	~ClassTree();

	SkillTreeElement* skillTree[TREE_SIZE];

	pugi::xml_document classFile;
	pugi::xml_document saveFile;

	//SDL_Rect rect = { 0, 0, 0, 0 };
	//iPoint drawP = { 0 , 0 };
	//SDL_Texture* treeTexture = nullptr;

public:
	PlayerClass playerClass = PlayerClass::REVENANT;

	//bool active = false;
};

#endif