#ifndef __SCENE_SWITCH_H__
#define __SCENE_SWITCH_H__

#include "GameObject.h"
class SceneSwitch : public GameObject
{
public:

	SceneSwitch(int destination, std::string name, std::string tag);

	~SceneSwitch();

    void OnCollisionEnter(PhysBody* col) override;

private:

    int destination = 0;
};

#endif // !__SCENE_SWITCH_H__