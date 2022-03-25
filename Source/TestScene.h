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

private:

	RoomManager roomManager;
	PlayerRevenant* player;
	//PlayerSage* player;

	ClassTree* revenantTree; //Test skill/spell tree
};

