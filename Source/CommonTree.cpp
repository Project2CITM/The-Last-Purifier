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

bool CommonTree::Upgrade(int id)
{

	return true;
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