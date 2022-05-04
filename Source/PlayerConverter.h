#pragma once
#include "GameObject.h"

class Trigger;
class HubScene;
class Text;

class PlayerConverter :
    public GameObject
{
public:

    PlayerConverter(iPoint position,std::string name);

    ~PlayerConverter();

    void Start()override;

    void PreUpdate()override;

    void Update()override;

    void PostUpdate()override;

    void CleanUp() override;

    void OnTriggerEnter(std::string trigger, PhysBody* col) override;

    void OnTriggerExit(std::string trigger, PhysBody* col) override;

    void setInside() { exterior = false; }

    void setOutside() { exterior = true; }

private:

    iPoint textPosition = { 0,0 };

    RenderObject RevenantClassSprite;

    RenderObject SageClassSprite;

    Trigger* trigger = nullptr;

    HubScene* pointerscene = nullptr;

    bool ReadyToChangeClass = false;

    bool playerHasalreadychanged = false;

    Text* text = nullptr;

    iPoint position;

    bool exterior = true;
};

