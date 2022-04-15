#ifndef __CLASS_TREE_H__
#define __CLASS_TREE_H__
#include <string>

#include "GameObject.h"
#include "Globals.h"
#include "Point.h"
#include "Player.h"
#include "External/PugiXml/src/pugixml.hpp"

#define TREE_SIZE 10
#define SAGE_TREE_XML "RevenantTree.xml"
#define REVENANT_TREE_XML "RevenantTree.xml"

enum class SkillLevel {
	LOCKED = 0,
	GREEN,
	BLUE,
	PURPLE,
	YELLOW
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

	fPoint position;

	SkillTreeElement(int id, std::string name, std::string description, int cost, int requiresID, fPoint position, SkillLevel maxLevel = SkillLevel::PURPLE)
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

class ClassTree 
{
public:

	static ClassTree* GetInstance();

	void Start();

	void PreUpdate();

	void Update();

	void PostUpdate();

	void ReleaseInstance();

	virtual void CleanUp();
public:
	SkillTreeElement* getSkillTree(int value);
	bool unlockSkill(int* classPoints, int skillId);
	bool levelUpCheck(SkillTreeElement* theSkill);

	//Loads the tree with all its upgrades
	bool LoadTree();

	//Saves the tree
	bool SaveTree();

	int getCurrentLevel(int id);

private:

	static ClassTree* instance;

	ClassTree(PlayerClass pClass);
	~ClassTree();

	SkillTreeElement* skillTree[TREE_SIZE];

	pugi::xml_document* classFile = new pugi::xml_document;

	//SDL_Rect rect = { 0, 0, 0, 0 };
	//iPoint drawP = { 0 , 0 };
	//SDL_Texture* treeTexture = nullptr;

public:
	PlayerClass playerClass = PlayerClass::REVENANT;

	//bool active = false;
};

#endif