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
	bool separador = false;
	int separadorCont = 0;
	int dialogCont = 1;

	std::string sentence[8];
	std::string advisorString="";

	Text* t;
	Text* advisor;
	//std::string sentence3 = "";

};

