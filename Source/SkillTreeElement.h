#pragma once

#include <string>


enum class SkillLevel {
	LOCKED = 0,
	NONE = 0,
	GREEN,
	BLUE,
	PURPLE,
	YELLOW
};


class SkillTreeElement
{
private:
	int id = 0;

	std::string name = "";
	std::string description = "";
	int cost = 100;

	SkillLevel currentLevel = SkillLevel::LOCKED;
	SkillLevel maxLevel = SkillLevel::PURPLE;

	int requiresID = 0;

public:
	SkillTreeElement(std::string name, std::string description, int cost, int requiresID, SkillLevel maxLevel = SkillLevel::PURPLE)
	{
		this->name = name;
		this->description = description;
		this->cost = cost;
		this->maxLevel = maxLevel;
	};
	~SkillTreeElement();

	int getId() {
		return id;
	};
	SkillLevel getCurrentLevel()
	{
		return currentLevel;
	}
	SkillLevel getMaxLevel()
	{
		return maxLevel;
	}
	int getCost()
	{
		return cost;
	};
	int getRequirement()
	{
		return requiresID;
	}
};

