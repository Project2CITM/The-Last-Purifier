#include "GameObject.h"
#include "ModuleEvents.h"
#include "Player.h"

class Trigger;

class OrbHP : public GameObject, EventListener
{
public:

	OrbHP(iPoint position);

	~OrbHP();

	void Start() override;

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void CleanUp() override;

	void OnTriggerEnter(std::string trigger, PhysBody* col) override;

	void GameEventTriggered(GameEvent id) override;

	void PlusHP();

private:
	Player* player = nullptr;

	Trigger* trigger = nullptr;
	Trigger* triggerDetectPlayer = nullptr;

};

