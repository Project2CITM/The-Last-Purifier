#ifndef _SCENE_GAME_
#define _SCENE_GAME_

#include "Scene.h"

class Player;

class SceneGame : public Scene
{
public:
	SceneGame(std::string name);

	Player* player = nullptr;

};
#endif
