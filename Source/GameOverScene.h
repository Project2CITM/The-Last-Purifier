#pragma once
#include "Scene.h"

class GameOverScene :
    public Scene
{
public:

    GameOverScene();

    ~GameOverScene();

    bool InitScene() override;

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

    RenderObject* pressKeyToTryAgain[2] = {nullptr};

    Application* app = nullptr;

    int score;
};

