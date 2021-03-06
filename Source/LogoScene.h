#ifndef __LOGO_SCENE_H__
#define __LOGO_SCENE_H__

#include "Scene.h"

class LogoScene : public Scene
{
public:

	// Constructor
	LogoScene();

	// Destructor
	~LogoScene();

	virtual bool InitScene();

	bool Start() override;

	bool PreUpdate() override;

	bool Update() override;

	bool PostUpdate() override;

	bool CleanUp() override;

private:
	void GaussEffect();

private:
	int count = 0;

	uint LogoAppear_FX = NULL;

	int drawLogo = 0;

	int gaussCount = 1;

	float gaussPower = 3.0f;
};

#endif //__LOGO_SCENE_H__