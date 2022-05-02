#pragma once
#include "GameObject.h"

class Trigger;
class HubScene;

class PlayerConverter :
    public GameObject
{
public:

    PlayerConverter(std::string name);

    ~PlayerConverter();

    void PreUpdate()override;

    void Update()override;

    void PostUpdate()override;

    void CleanUp() override;

    void OnTriggerEnter(std::string trigger, PhysBody* col) override;

    void OnTriggerExit(std::string trigger, PhysBody* col) override;

private:

    RenderObject RevenantClassSprite;

    RenderObject SageClassSprite;

    Trigger* trigger = nullptr;

    HubScene* pointerscene = nullptr;

    bool ReadyToChangeClass = false;
};

