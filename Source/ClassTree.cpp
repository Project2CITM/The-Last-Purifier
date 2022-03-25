#include "ClassTree.h"
#include <iostream>

ClassTree::ClassTree(PlayerClass pClass) : GameObject(name, tag)
{
	playerClass = pClass;
}

ClassTree::~ClassTree()
{

}

void ClassTree::Start()
{
	pugi::xml_parse_result result;
	switch (playerClass)
	{
	case PlayerClass::REVENANT:
		result = classFile->load_file(REVENANT_TREE_XML);
		break;
	case PlayerClass::SAGE:
		result = classFile->load_file(SAGE_TREE_XML);
		break;
	}

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", REVENANT_TREE_XML, result.description());

	LoadTree();
}

void ClassTree::PreUpdate()
{

}

void ClassTree::Update()
{
	//std::cout << "A Skill name ->" << skillTree[3]->name << std::endl;
}

void ClassTree::PostUpdate()
{

}

void ClassTree::CleanUp()
{
	RELEASE(classFile);
	
	for (int i = 0; i < TREE_SIZE; i++)
	{
		RELEASE(skillTree[i]);
	}
}

bool ClassTree::unlockSkill(int* classPoints, int skillId)
{
	SkillTreeElement* theSkill = getSkillTree(skillId);
	if (theSkill == nullptr) return false;

	SkillTreeElement* theRequiredSkill = getSkillTree(theSkill->requiresID);
	if (theRequiredSkill == nullptr) return false;
	
	//Checks if the skill with the given id is already maxed(level)
	if (theSkill->currentLevel < theSkill->maxLevel)
	{
		//Checks if the required skill is already unlocked
		if (theRequiredSkill->currentLevel >= SkillLevel::BLUE)
		{
			//Checks if there are enough points to unlock
			if (*classPoints <= theSkill->cost)
			{   //Unlocks
				theSkill->currentLevel = (SkillLevel) ((int)theSkill->currentLevel + 1);
				classPoints -= theSkill->cost;
				
				return true;
			}
		}
	}

	//Return false means that there is nothing to unlock.
	return false;
}

bool ClassTree::LoadTree()
{
	pugi::xml_node bNode = classFile->child("class_tree").first_child();
	
	for (int i = 0; i < TREE_SIZE; i++)
	{
		fPoint temp = { bNode.child("position").attribute("x").as_float(), bNode.child("position").attribute("y").as_float() };
		skillTree[i] = new SkillTreeElement(
			bNode.attribute("id").as_int(),
			(std::string) bNode.child_value("name"),
			(std::string) bNode.child_value("description"),
			(int) bNode.child_value("cost"),
			(int) bNode.child_value("requiresID"),
			temp,
			(SkillLevel) (int) bNode.child_value("maxLevel")
		);

		bNode = bNode.next_sibling();
	}

	return true;
}

/*
  GETTERS AND SETTERS
*/

SkillTreeElement* ClassTree::getSkillTree(int value)
{
	return skillTree[value];
}
