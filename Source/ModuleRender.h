#ifndef __MODULE_RENDER_H__
#define __MODULE_RENDER_H__

#include <vector>
#include "Module.h"
#include "RenderObject.hpp"
#include "Camera.h"

#define MAX_RENDERLAYERS 5

using namespace std;

struct RenderLayer
{
	bool sort = false;
	std::vector<RenderObject> renderObjects;
};

class ModuleRender : public Module
{
public:
	SDL_Renderer* renderer = nullptr;
	Camera* camera = nullptr;

	//int zoom = 1;

	SDL_DisplayMode displayMode;
public:
	ModuleRender(Application* app, bool start_enabled = true);
	~ModuleRender();

	bool Init(pugi::xml_node&);
	UpdateStatus PreUpdate() override;
	UpdateStatus Update() override;
	UpdateStatus PostUpdate() override;
	bool CleanUp();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="texture"></param>
	/// <param name="pos"></param>
	/// <param name="section"></param>
	/// <param name="scale"></param>
	/// <param name="layer">: If it is outside the range of the layers, it will limit within the layer</param>
	/// <param name="orderInlayer"></param>
	/// <param name="rotation"></param>
	/// <param name="flip"></param>
	/// <param name="speed"></param>
	void AddTextureRenderQueue(SDL_Texture* texture, iPoint pos, SDL_Rect section = {0,0,0,0}, float scale = 1, int layer = 0, float orderInlayer = 0.0f, float rotation = 0, SDL_RendererFlip flip = SDL_FLIP_NONE, float speed = 1.0f);// Speed = 1.0f = Fullscreen camera
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="rect"></param>
	/// <param name="color"></param>
	/// <param name="filled"></param>
	/// <param name="layer">: If it is outside the range of the layers, it will limit within the layer</param>
	/// <param name="orderInlayer"></param>
	/// <param name="speed"></param>
	void AddRectRenderQueue(const SDL_Rect& rect, SDL_Color color = { 0,0,0,255 }, bool filled = true, int layer = 1, float orderInlayer = 0.0f, float speed = 1.0f);
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="radius"></param>
	/// <param name="color"></param>
	/// <param name="layer">: If it is outside the range of the layers, it will limit within the layer</param>
	/// <param name="orderInLayer"></param>
	/// <param name="speedRegardCamera"></param>
	void AddCircleRenderQueue(const iPoint pos, int radius, SDL_Color color = { 0,0,0,255 }, int layer = 0, float orderInLayer = 0.0f, float speedRegardCamera = 1.0f);
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="pos1"></param>
	/// <param name="pos2"></param>
	/// <param name="adjust"></param>
	/// <param name="color"></param>
	/// <param name="layer">: If it is outside the range of the layers, it will limit within the layer</param>
	/// <param name="orderInLayer"></param>
	/// <param name="speedRegardCamera"></param>
	void AddLineRenderQueue(iPoint pos1, iPoint pos2, bool adjust = true, SDL_Color color = { 0,0,0,255 }, int layer = 0, float orderInLayer = 0.0f, float speedRegardCamera = 1.0f);
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="renderObject">: If it is outside the range of the layers, it will limit within the layer</param>
	void AddRenderObjectRenderQueue(RenderObject renderObject);

	void ClearRederQueue();

	/// <summary>
	/// Detect if the rect is in the screen
	/// </summary>
	/// <param name="rect"></param>
	/// <returns></returns>
	bool InScreen(const SDL_Rect& rect);

	int RoundToInt(int num);

	void ToggleVsync(bool vsync);

private:
	std::vector<RenderLayer> renderLayers;

	uint uiLayer = MAX_RENDERLAYERS - 1;;

	int gamePixels = 16; // The pixels per texutre used on this game

private:
	void SortingObjectsInLayer(vector<RenderObject>& obj);
	void GetSaveData(pugi::xml_document& save) override;
};

#endif // !__MODULE_RENDER_H__