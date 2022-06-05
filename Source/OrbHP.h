#include "GameObject.h"
#include "ModuleEvents.h"

class OrbHP : public GameObject, EventListener
{
public:

	OrbHP();

	~OrbHP();

	void Start() override;

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void CleanUp() override;

	void OnTriggerEnter(std::string trigger, PhysBody* col) override;

	void PlusHP();

	//Player* player = nullptr;


private:

};

