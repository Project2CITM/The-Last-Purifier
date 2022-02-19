#include "Scene.h"

#include "RoomManager.h"

class TestScene :  public Scene
{
public:

	TestScene(Application* app);

	~TestScene();

	bool Start() override;

	bool PreUpdate() override;

	bool Update() override;

	bool PostUpdate() override;

	bool CleanUp() override;

private:

	RoomManager roomManager = RoomManager(_app);
};

