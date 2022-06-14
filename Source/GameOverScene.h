#pragma once
#include "Scene.h"

class GUIButton;

class GameOverScene :
    public Scene
{
public:

    GameOverScene();

    ~GameOverScene();

    bool Start() override;

    bool PreUpdate() override;

    bool Update() override;

    bool PostUpdate() override;

    bool CleanUp() override;
    
private:

    Text* text = nullptr;

    Text* text2 = nullptr;

    Text* text5 = nullptr;

    RenderObject* gameOverBG = nullptr;

    RenderObject* souls = nullptr;

    RenderObject* pressKeyToTryAgain[2] = {nullptr};

    Application* app = nullptr;

    GUIButton* retryButton = nullptr;
    GUIButton* mainMenuButton = nullptr;

    int score = 0;

    int selectedOption = 0;
};

