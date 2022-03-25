#include "ClassTree.h"

ClassTree::ClassTree()
{
	LoadTree();
}

ClassTree::~ClassTree()
{

}

SkillTreeElement* ClassTree::getSkillTree(int value)
{
	return skillTree[value];
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

//bool ClassTree::levelUpSkill()
//{
//
//}

bool ClassTree::LoadTree()
{
	LoadBaseTree();
}

bool ClassTree::LoadBaseTree()
{
	skillTree[0] = new SkillTreeElement(0, "Cover", "Takes cover", 100, 0);
}
