#include "CommonTree.h"

CommonTree::CommonTree() : GameObject(name, tag)
{

}

CommonTree::~CommonTree()
{

}

void CommonTree::Start()
{
	//Loads Values
	pugi::xml_parse_result result;
	treeFile->load_file(COMMON_TREE_XML);
	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", COMMON_TREE_XML, result.description());

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

void CommonTree::CleanUp()
{
	RELEASE(treeFile);
}

bool CommonTree::LoadDictionary()
{
	pugi::xml_node bNode = treeFile->child("common_tree").child("dictionary").first_child();

	while (bNode.value() != NULL)
	{
		upgradesDic.insert({ (CommonUpgrades) bNode.attribute("id").as_int(), (float) bNode.attribute("value").as_int() });

		bNode = bNode.next_sibling();
	}

	return true;
}

bool CommonTree::LoadTree()
{
	pugi::xml_node bNode = treeFile->child("common_tree").child("elements");

	while (bNode.value() != NULL)
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

bool CommonTree::SaveTree()
{


	return true;
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