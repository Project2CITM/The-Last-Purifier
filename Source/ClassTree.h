#ifndef __CLASS_TREE_H__
#define __CLASS_TREE_H__
#include <string>
#define TREE_SIZE 10

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

	SkillTreeElement(int id, std::string name, std::string description, int cost, int requiresID, SkillLevel maxLevel = SkillLevel::PURPLE)
	{
		this->id = id;
		this->name = name;
		this->description = description;
		this->cost = cost;
		this->requiresID = requiresID;

		this->maxLevel = maxLevel;
	};
};


class ClassTree
{
public:


protected:
	SkillTreeElement* skillTree[TREE_SIZE];

public:
	ClassTree();
	~ClassTree();


	SkillTreeElement* getSkillTree(int value);
	bool unlockSkill(int* classPoints, int skillId);
	bool levelUpCheck(SkillTreeElement* theSkill);
	
	bool LoadTree();
	bool LoadBaseTree();
};

#endif