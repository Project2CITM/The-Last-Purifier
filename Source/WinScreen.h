#pragma once
#include "Scene.h"
class WinScene :
    public Scene
{
public:

    WinScene();

    ~WinScene();

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

    RenderObject* winBG = nullptr;

    RenderObject* soul = nullptr;

    RenderObject* pressKeyToReturnToHub[2] = { nullptr };

    Application* app = nullptr;

    int score;

};


