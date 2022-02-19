#include "Application.h"
#include <string.h>

int cameraSpeed = 1;

ModuleRender::ModuleRender(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "renderer";
	renderer = NULL;

	camera = new Camera(App);
	camera->x = camera->y = 0;

	renderLayers.resize(4);
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
	SDL_SetRenderDrawColor(renderer, 0, 0, 1, 0);
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
	// Sorting layers
	for (int i = 0; i < renderLayers.size(); i++)
	{
		SortRenderObjects(renderLayers[i]);
	}

	//Draw
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0, count = renderLayers[i].size(); j < count; ++j)
		{
			renderLayers[i][j].Draw(renderer);
		}
	}

	//if (App->debug->debugCollisionView)
	//{
	//	App->physics->ShapesRender();
	//}
	
	SDL_RenderPresent(renderer);

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
	SDL_Rect destRect = { 0,0 };

	RenderObject renderObject;

	renderObject.InitAsTexture(texture, destRect, section, layer, orderInlayer, flip, rotation, scale, speed);

	if (layer == 2 || layer == 3) renderObject.speedRegardCamera = 0;	//If texture in UI layer, it moves alongside the camera-> Therefor, speed = 0;

	renderObject.destRect.x = (int)(-camera->x * speed) + pos.x * App->window->scale * zoom;
	renderObject.destRect.y = (int)(-camera->y * speed) + pos.y * App->window->scale * zoom;

	if (section.h != 0 && section.w != 0)
	{
		renderObject.destRect.w = section.w;
		renderObject.destRect.h = section.h;
	}
	else
	{
		// Collect the texture size into rect.w and rect.h variables
		SDL_QueryTexture(texture, nullptr, nullptr, &renderObject.destRect.w, &renderObject.destRect.h);
	}

	renderObject.destRect.w *= scale * App->window->scale * zoom;
	renderObject.destRect.h *= scale * App->window->scale * zoom;

	//LOG("direction in memory : %#x", renderObject.texture);
	try 
	{
		renderLayers[layer].push_back(renderObject);
	}
	catch (const exception& e)
	{
		LOG("ERRRROR");
	}	
}

void ModuleRender::AddTextureRenderQueue(RenderObject obj)
{
	RenderObject object = obj;

	object.destRect.x = (int)(-camera->x * object.speedRegardCamera) + object.destRect.x * App->window->scale;
	object.destRect.y = (int)(-camera->y * object.speedRegardCamera) + object.destRect.y * App->window->scale;

	if (object.section.w != 0 && object.section.h != 0)
	{
		object.destRect.w = object.section.w;
		object.destRect.h = object.section.h;
	}
	else
	{
		// Collect the texture size into rect.w and rect.h variables
		SDL_QueryTexture(object.texture, nullptr, nullptr, &object.destRect.w, &object.destRect.h);
	}

	object.destRect.w *= object.scale * App->window->scale;
	object.destRect.h *= object.scale * App->window->scale;

	renderLayers[object.layer].push_back(object);
}

void ModuleRender::AddRectRenderQueue(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int layer, float orderInlayer, bool filled, float speed)
{
	RenderObject renderR;

	SDL_Rect rec = { (-camera->x * speed) + rect.x * App->window->scale, (-camera->y * speed) + rect.y * App->window->scale,
		rect.w * App->window->scale, rect.h * App->window->scale };

	renderR.InitAsRect(rec, { r,g,b,a }, filled, layer, orderInlayer, speed);

	renderLayers[layer].push_back(renderR);
}

void ModuleRender::ClearRederQueue()
{
	for (int i = 0; i < 4; i++)
	{
		renderLayers[i].clear();
	}
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

void ModuleRender::SortRenderObjects(vector<RenderObject> &obj)
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

void ModuleRender::CameraMove(iPoint pos)
{
	camera->x = pos.x + (SCREEN_WIDTH / 2);	//	Camera position = target position

	camera->y = pos.y;
}


void ModuleRender::GetSaveData(pugi::xml_document& save)
{
	pugi::xml_node n = save.child("game_state").child("renderer");

	n.child("camera").attribute("x") = camera->x;
	n.child("camera").attribute("y") = camera->y;
}

#pragma region OBSOLETE NOT USE!!!!!

// Blit to screen
bool ModuleRender::Blit(SDL_Texture* texture, int x, int y, float scale, SDL_Rect* section, float speed, double angle, SDL_RendererFlip flip, int pivot_x, int pivot_y)
{
	bool ret = true;
	SDL_Rect rect;
	rect.x = (int) (camera->x * speed) + x * SCREEN_SIZE;
	rect.y = (int) (camera->y * speed) + y * SCREEN_SIZE;

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= SCREEN_SIZE;
	rect.h *= SCREEN_SIZE;

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if(pivot_x != INT_MAX && pivot_y != INT_MAX)
	{
		pivot.x = pivot_x;
		pivot.y = pivot_y;
		p = &pivot;
	}

	if(SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flip) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera)
	{
		rec.x = (int)(camera->x + rect.x * App->window->scale);
		rec.y = (int)(camera->y + rect.y * App->window->scale);
		rec.w *= App->window->scale;
		rec.h *= App->window->scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);
	
	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool adjust, bool use_camera)
{
	bool ret = true;

	if (gamePixels != 0 && adjust)
	{
		x1 = RoundToInt(x1);
		y1 = RoundToInt(y1);
		x2 = RoundToInt(x2);
		y2 = RoundToInt(y2);
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, -camera->x + x1 * App->window->scale, -camera->y + y1 * App->window->scale, -camera->x + x2 * App->window->scale, -camera->y + y2 * App->window->scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * App->window->scale, y1 * App->window->scale, x2 * App->window->scale, y2 * App->window->scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float) M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(-camera->x + x * App->window->scale + radius * cos(i * factor) * App->window->scale);
		points[i].y = (int)(-camera->y + y * App->window->scale + radius * sin(i * factor) * App->window->scale);
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

#pragma endregion