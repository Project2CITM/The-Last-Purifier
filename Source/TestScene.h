#include "Scene.h"

#include "RoomManager.h"

class PlayerRevenant;
class PlayerSage;
class ClassTree;

class TestScene :  public Scene
{
public:

	TestScene();

	~TestScene();

	bool Start() override;

	bool PreUpdate() override;

	bool Update() override;

	bool PostUpdate() override;

	bool CleanUp() override;

	void chargeDialog();

private:

	RoomManager roomManager;
	PlayerRevenant* player;
	//PlayerSage* player;

	ClassTree* revenantTree; //Test skill/spell tree7


	//Dialog
	pugi::xml_node configDialog;
	bool dialogEnable = false;

	std::string sentence1 = "";
	std::string sentence2 = "";
	std::string sentence3 = "";

};

