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

	//load the texture
	void LoadTexFile(const pugi::xml_document& dataFile);

	//load the fx
	void LoadFxFile(const pugi::xml_document& dataFile);

	//load and play the music
	void LoadMusFile(const pugi::xml_document& dataFile);

private:
	int count = 0;

	uint LogoAppear_FX = NULL;

	int drawLogo = 0;

	int gaussCount = 1;

	float gaussPower = 3.0f;

	SDL_Texture* Team_Logo = nullptr;
	SDL_Texture* Game_Logo = nullptr;

};

#endif //__LOGO_SCENE_H__