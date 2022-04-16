#pragma once
#include "GameObject.h"
class SceneSwitch : public GameObject
{
public:
	SceneSwitch(int destination, std::string name, std::string tag);
	~SceneSwitch();

    virtual void CleanUp();

    void OnCollisionEnter(PhysBody* col) override;
    void OnCollisionExit(PhysBody* col) override;

private:

    int destination = 0;

};

