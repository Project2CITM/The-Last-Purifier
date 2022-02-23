#include "Application.h"
#include <string.h>

int cameraSpeed = 1;

ModuleRender::ModuleRender(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "renderer";
	renderer = NULL;

	camera = new Camera(App);
	camera->x = camera->y = 0;

	// Init renderLayers
	renderLayers.resize(MAX_RENDERLAYERS);
}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init(pugi::xml_node& config)
{
	LOG("Creating Renderer context");
	bool ret = true;
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	this->config = config;
	
	if (App->saveF.child("game_state").child("settings").attribute("vSync").as_bool(false))
	{
		App->vsync = true;
	}

	flags |= SDL_RENDERER_PRESENTVSYNC;

	renderer = SDL_CreateRenderer(App->window->window, -1, flags);
	
	if (renderer == NULL)
	{
		LOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	camera->Start();

	SDL_GetCurrentDisplayMode(0, &displayMode);

	return ret;
}

// PreUpdate: clear buffer
UpdateStatus ModuleRender::PreUpdate()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	return UPDATE_CONTINUE;
}

// Update: debug camera
UpdateStatus ModuleRender::Update()
{	
	camera->Update();

	if (App->FullScreenDesktop)
	{
		// Fullscreen
		SDL_RenderSetLogicalSize(renderer, App->window->width, App->window->height);
	}
	else
	{
		SDL_RenderSetLogicalSize(renderer, App->window->width, App->window->height);
	}

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UpdateStatus ModuleRender::PostUpdate()
{
	App->physics->ShapesRender();

	// Sorting layers
	for (int i = 0; i < renderLayers.size(); ++i)
	{
		if (renderLayers[i].sort) SortingObjectsInLayer(renderLayers[i].renderObjects);
	}

	//Draw
	for (int i = 0; i < MAX_RENDERLAYERS; ++i)
	{
		for (int j = 0, length = renderLayers[i].renderObjects.size(); j < length; ++j)
		{
			renderLayers[i].renderObjects[j].Draw(renderer);
		}
	}

	//if (App->debug->debugCollisionView)
	//{
	//	App->physics->ShapesRender();
	//}
	
	// Update the screen
	SDL_RenderPresent(renderer);

	// Clear layers
	ClearRederQueue();

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	RELEASE(camera);

	//Destroy window
	if(renderer != NULL)
	{
		SDL_DestroyRenderer(renderer);
	}

	return true;
}

void ModuleRender::AddTextureRenderQueue(SDL_Texture* texture, iPoint pos, SDL_Rect section, float scale, int layer, float orderInlayer, float rotation, SDL_RendererFlip flip, float speed)
{
	if (section.w == 0 || section.h == 0)
	{
		// Collect the texture size into section.w and section.h variables
		SDL_QueryTexture(texture, nullptr, nullptr, &section.w, &section.h);
	}

	if (!InScreen(SDL_Rect{ pos.x, pos.y, section.w, section.h })) return;
	SDL_Rect destRect = { 0,0 };

	RenderObject renderObject;

	//If texture in UI layer, it moves alongside the camera-> , speed = 0;
	if (uiLayer >= 0 && layer == uiLayer) speed = 0;

	renderObject.InitAsTexture(texture, destRect, section, layer, orderInlayer, flip, rotation, scale, speed);

	renderObject.destRect.x = (int)(-camera->x * speed) + pos.x * App->window->scale * zoom;
	renderObject.destRect.y = (int)(-camera->y * speed) + pos.y * App->window->scale * zoom;

	renderObject.destRect.w = section.w;
	renderObject.destRect.h = section.h;

	renderObject.destRect.w *= scale * App->window->scale * zoom;
	renderObject.destRect.h *= scale * App->window->scale * zoom;

	//LOG("direction in memory : %#x", renderObject.texture);
	renderLayers[layer].renderObjects.push_back(renderObject);
}
	
void ModuleRender::AddRectRenderQueue(const SDL_Rect& rect, SDL_Color color, int layer, float orderInlayer, bool filled, float speed)
{
	RenderObject renderR;

	//If texture in UI layer, it moves alongside the camera-> , speed = 0;
	if (uiLayer >= 0 && layer == uiLayer) speed = 0;

	if (!InScreen(rect)) return;

	SDL_Rect rec = { (-camera->x * speed) + rect.x * App->window->scale, (-camera->y * speed) + rect.y * App->window->scale,
		rect.w * App->window->scale, rect.h * App->window->scale };

	renderR.InitAsRect(rec, { color.r,color.g,color.b,color.a }, filled, layer, orderInlayer, speed);

	renderLayers[layer].renderObjects.push_back(renderR);
}

void ModuleRender::AddCircleRenderQueue(const iPoint pos, int radius, SDL_Color color, int layer, float orderInLayer, float speed)
{
	RenderObject renderC;

	//If texture in UI layer, it moves alongside the camera-> , speed = 0;
	if (uiLayer >= 0 && layer == uiLayer) speed = 0;

	renderC.InitAsCircle(pos, radius, color, layer, orderInLayer, speed);

	float factor = (float)M_PI / 180.0f;

	for (uint i = 0; i < 360; ++i)
	{
		renderC.points[i].x = (int)(-camera->x + renderC.destRect.x * App->window->scale + radius * cos(i * factor) * App->window->scale);
		renderC.points[i].y = (int)(-camera->y + renderC.destRect.y * App->window->scale + radius * sin(i * factor) * App->window->scale);
	}

	renderLayers[layer].renderObjects.push_back(renderC);
}

void ModuleRender::AddLineRenderQueue(iPoint pos1, iPoint pos2, bool adjust, SDL_Color color, int layer, float orderInLayer, float speed)
{
	RenderObject renderL;

	//If texture in UI layer, it moves alongside the camera-> , speed = 0;
	if (uiLayer >= 0 && layer == uiLayer) speed = 0;

	if (gamePixels != 0 && adjust)
	{
		pos1.x = RoundToInt(pos1.x);
		pos1.y = RoundToInt(pos1.y);
		pos2.x = RoundToInt(pos2.x);
		pos2.y = RoundToInt(pos2.y);
	}

	pos1.x = -camera->x + pos1.x * App->window->scale;
	pos1.y = -camera->y + pos1.y * App->window->scale;
	pos2.x = -camera->x + pos2.x * App->window->scale;
	pos2.y = -camera->y + pos2.y * App->window->scale;

	renderL.InitAsLine(pos1, pos2, adjust, color, layer, orderInLayer, speed);

	renderLayers[layer].renderObjects.push_back(renderL);
}

void ModuleRender::AddRenderObjectRenderQueue(RenderObject renderObject)
{
	// If texture in UI layer, it moves alongside the camera-> , speed = 0;
	if (uiLayer >= 0 && renderObject.layer == uiLayer) renderObject.speedRegardCamera = 0;
	
	// Adjust destination position using camera and screen size
	renderObject.destRect.x = (int)(-camera->x * renderObject.speedRegardCamera) + renderObject.destRect.x * App->window->scale;
	renderObject.destRect.y = (int)(-camera->y * renderObject.speedRegardCamera) + renderObject.destRect.y * App->window->scale;

	switch (renderObject.type)
	{
	case RENDER_TEXTURE:
	{
		if (renderObject.section.h != 0 && renderObject.section.w != 0)
		{
			renderObject.destRect.w = renderObject.section.w;
			renderObject.destRect.h = renderObject.section.h;
		}
		else
		{
			// Collect the texture size into rect.w and rect.h variables
			SDL_QueryTexture(renderObject.texture, nullptr, nullptr, &renderObject.destRect.w, &renderObject.destRect.h);
		}
		renderObject.destRect.w *= renderObject.scale * App->window->scale;
		renderObject.destRect.h *= renderObject.scale * App->window->scale;
		break;
	}
	case RENDER_CIRCLE:
	{
		float factor = (float)M_PI / 180.0f;
		for (uint i = 0; i < 360; ++i)
		{
			renderObject.points[i].x = (int)(-camera->x + renderObject.destRect.x * App->window->scale + renderObject.radius * cos(i * factor) * App->window->scale);
			renderObject.points[i].y = (int)(-camera->y + renderObject.destRect.y * App->window->scale + renderObject.radius * sin(i * factor) * App->window->scale);
		}
		break;
	}
	case RenderType::RENDER_LINE:
	{
		if (gamePixels != 0 && renderObject.adjust)
		{
			renderObject.pos1.x = RoundToInt(renderObject.pos1.x);
			renderObject.pos1.y = RoundToInt(renderObject.pos1.y);
			renderObject.pos2.x = RoundToInt(renderObject.pos2.x);
			renderObject.pos2.y = RoundToInt(renderObject.pos2.y);
		}
		renderObject.pos1.x = -camera->x + renderObject.pos1.x * App->window->scale;
		renderObject.pos1.y = -camera->y + renderObject.pos1.y * App->window->scale;
		renderObject.pos2.x = -camera->x + renderObject.pos2.x * App->window->scale;
		renderObject.pos2.y = -camera->y + renderObject.pos2.y * App->window->scale;
		break;
	}
	}

	renderLayers[renderObject.layer].renderObjects.push_back(renderObject);
}

void ModuleRender::SortingObjectsInLayer(vector<RenderObject>& obj)
{
	int less = 0;
	int objSize = obj.size();

	for (int i = 0; i < objSize; ++i)
	{
		less = i;
		for (int j = i; j < objSize; ++j)
		{
			if (obj[j].orderInLayer < obj[less].orderInLayer)
			{
				swap(obj[j], obj[less]);
			}
		}
	}
}

void ModuleRender::ClearRederQueue()
{
	for (int i = 0; i < MAX_RENDERLAYERS; ++i)
	{
		renderLayers[i].renderObjects.clear();
	}
}

bool ModuleRender::InScreen(const SDL_Rect& rect)
{
	return true;

	/*int a1 = rect.x + rect.w;
	int a2 = rect.x;
	int a3 = rect.y + rect.h;
	int a4 = rect.y;

	int b1 = camera->x;
	int b2 = camera->x + App->window->width;
	int b3 = camera->y;
	int b4 = camera->y + App->window->height;

	if (a1 < b1 || a2 > b2 ||
		a3 < b3 || a4 > b4)
	{
		return false;
	}*/

	/*if (rect.x + rect.w < camera->x || rect.x > camera->x + App->window->width ||
		rect.y + rect.h < camera->y || rect.y > camera->y + App->window->height)
	{
		return false;
	}*/

	return true;
}

int ModuleRender::RoundToInt(int num)
{
	float divisionRes;
	divisionRes = (float)num / (float)gamePixels;

	divisionRes = lround((double)divisionRes);

	return (int)(divisionRes * gamePixels);
}

void ModuleRender::ToggleVsync(bool vsync)
{
	App->vsync = vsync;
}

void ModuleRender::GetSaveData(pugi::xml_document& save)
{
	pugi::xml_node n = save.child("game_state").child("renderer");

	n.child("camera").attribute("x") = camera->x;
	n.child("camera").attribute("y") = camera->y;
}