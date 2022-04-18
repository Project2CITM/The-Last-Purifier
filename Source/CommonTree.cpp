#include "CommonTree.h"
#include "ModuleScene.h"
#include "SceneGame.h"

CommonTree* CommonTree::instance = nullptr;

CommonTree::CommonTree()
{
	this->listenTo = GameEvent::SAVE_GAME;

	Application::GetInstance()->events->AddListener(this);

	Start();
}

CommonTree::~CommonTree()
{

}

CommonTree* CommonTree::GetInstance()
{
	if (instance == nullptr)
	{
		SceneGame* scene = (SceneGame*)Application::GetInstance()->scene->scenes[Application::GetInstance()->scene->currentScene];
		instance = new CommonTree();
	}
	return instance;
}

void CommonTree::Start()
{
	//Loads Values
	pugi::xml_parse_result result1;
	pugi::xml_parse_result result2;

	result1 = treeFile.load_file(COMMON_TREE_XML);
	result2 = saveFile.load_file(SAVE_COMMON_TREE_XML);
	if (result1 == NULL) LOG("Could not load xml file: %s. pugi error: %s", COMMON_TREE_XML, result1.description());
	if (result2 == NULL) LOG("Could not load xml file: %s. pugi error: %s", SAVE_COMMON_TREE_XML, result2.description());

	LoadBaseTree();
	LoadDictionary();
	SaveLoadTree(true);
}

void CommonTree::PreUpdate()
{
	
}

void CommonTree::Update()
{
	
}

void CommonTree::PostUpdate()
{

}

void CommonTree::ReleaseInstance()
{
	if (instance != nullptr)
	{
		CleanUp();
		delete instance;
		instance = nullptr;
	}
}

void CommonTree::CleanUp()
{
	treeList->clearPtr();
	RELEASE(treeList);
}

bool CommonTree::LoadDictionary()
{
	pugi::xml_node bNode = treeFile.child("common_tree").child("dictionary").first_child();

	while (bNode != NULL)
	{
		upgradesDic.insert({ (CommonUpgrades) bNode.attribute("id").as_int(), (float) bNode.attribute("value").as_int() });
		unlockedDic.insert({ (CommonUpgrades)bNode.attribute("id").as_int(), 0 });
		bNode = bNode.next_sibling();
	}

	return true;
}

bool CommonTree::LoadBaseTree()
{
	pugi::xml_node bNode = treeFile.child("common_tree").child("elements").first_child();

	while (bNode != NULL)
	{
		treeList->add(new TreeElement(
			bNode.attribute("id").as_int(),
			(CommonUpgrades)bNode.attribute("type").as_int(),
			bNode.child("requires").attribute("id1").as_int(),
			bNode.child("requires").attribute("id2").as_int(),
			bNode.child("unlocked").attribute("value").as_float()
		));

		bNode = bNode.next_sibling();
	}

	return true;
}

bool CommonTree::SaveLoadTree(bool load)
{
	if (saveFile == nullptr) return false;

	pugi::xml_node bNode = saveFile.first_child().first_child();

	ListItem<TreeElement*>* element = treeList->start;

	while (element != NULL)
	{
		if (load) //Loads
		{
			element->data->unlocked = bNode.attribute("unlocked").as_bool();
		}
		else //Saves
		{
			bNode.attribute("unlocked") = element->data->unlocked;
		}

		bNode = bNode.next_sibling();
		element = element->next;
	}


	if (!load) //In case is called as a Save, it saves it back to the file
	{
		saveFile.save_file(SAVE_COMMON_TREE_XML);
	}


	return true;
}

void CommonTree::GameEventTriggered()
{
	SaveLoadTree();
}

bool CommonTree::Upgrade(int id)
{
	TreeElement* element = getElement(id);
	int req1 = element->requiresID1;
	int req2 = element->requiresID2;

	bool toReturn = true;

	if (req1 != -1)
	{
		if (!getElement(req1)->unlocked)
		{
			toReturn = false;
		}
	}

	if (req2 != -1)
	{
		if (!getElement(req2)->unlocked)
		{
			toReturn = false;
		}
	}

	element->unlocked = toReturn;

	//Increases the bonus attribute of an upgrade
	IncreaseValue((CommonUpgrades) element->type);

	return toReturn;
}

TreeElement* CommonTree::getElement(int id)
{
	ListItem<TreeElement*>* element = treeList->start;

	while (element != NULL)
	{
		if (element->data->id == id)
		{
			return element->data;
		}
		element = element->next;
	}

	return nullptr;
}

float CommonTree::getValue(CommonUpgrades id)
{
	// Look for the value of the given CommonUpgrade
	for (auto it = unlockedDic.begin(); it != unlockedDic.end(); ++it)
	{
		if (it->first == id) return it->second;
	}
	// Return -1 if not found
	return 0;
}

bool CommonTree::CheckUpgrades()
{
	bool toReturn = true;

	ListItem<TreeElement*>* element = treeList->start;

	while (element != NULL)
	{
		if (element->data->unlocked)
		{
			IncreaseValue((CommonUpgrades) element->data->type);
		}
		element = element->next;
	}

	return toReturn;
}

void CommonTree::IncreaseValue(CommonUpgrades id)
{
	unlockedDic.at(id) += upgradesDic.at(id);
}