#ifndef __CLASS_TREE_H__
#define __CLASS_TREE_H__
#include <string>

#include "GameObject.h"
#include "Globals.h"
#include "Point.h"
#include "Player.h"
#include "External/PugiXml/src/pugixml.hpp"
#include "ModuleEvents.h"
#include <map>

#define TREE_SIZE 20
#define MAX_SPELLL_BY_CLASS 10
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

	void GameEventTriggered(GameEvent id) override;

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
	ClassTree(PlayerClass pClass);
	~ClassTree();

	//SkillLevel ResolveLevel(std::string input);

private:

	static ClassTree* instance;

	//std::map<PlayerClass, SkillTreeElement* [TREE_SIZE]> skillTree {};
	SkillTreeElement* skillTree[TREE_SIZE] = {};

	pugi::xml_document classFile;
	pugi::xml_document saveFile;

public:
	PlayerClass playerClass = PlayerClass::REVENANT;

	//bool active = false;
};

#endif