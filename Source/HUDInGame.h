#ifndef __HUD_IN_GAME_H__
#define __HUD_IN_GAME_H__

#include "Scene.h"

class HUDInGame : public Scene
{
public:

	//Constructor
	HUDInGame();

	//Destructor
	~HUDInGame();

	virtual bool InitScene();

	bool Start() override;

	bool PreUpdate() override;

	bool Update() override;

	bool PostUpdate() override;

	bool CleanUp() override;

private:

};

#endif //__HUD_IN_GAME_H__