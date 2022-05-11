#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "AssetsManager.h"
#include "External/SDL_image/include/SDL_image.h"

ModuleTextures::ModuleTextures(bool start_enabled) : Module(start_enabled)
{
	name = "textures";
}

// Destructor
ModuleTextures::~ModuleTextures()
{}

// Called before render is available
bool ModuleTextures::Init(pugi::xml_node& config)
{
	LOG("Init Image library");
	bool ret = true;

	this->config = config;

	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	SDL_Surface* iconSurface = IMG_Load("Assets/textures/MainCharacters/VirtualGuy/Fall(32x32).png");

	app->window->SetWindowIcon(iconSurface);

	return ret;
}

// Called before quitting
bool ModuleTextures::CleanUp()
{
	LOG("Freeing textures and Image library");

	//ListItem<SDL_Texture*>* item = textures.start;

	for (int i = 0, count = textures.count(); i < count; i++)
	{
		LOG("Freeing textures and Image library num: %d / %d || direccion in memory: %#x || path: %s\n",i, count, textures[i], texturePath[i].path);
		if (textures[i] != nullptr)
		{
			SDL_DestroyTexture(textures[i]);
			textures[i] = nullptr;
		}
		LOG("Succeful clean texture");
	}

	if (screenTex != nullptr)
	{
		SDL_DestroyTexture(screenTex);
		screenTex = nullptr;
		LOG("Succeful clean screenTex");
	}

	textures.clear();

	texturePath.clear();

	IMG_Quit();
	return true;
}

//bool ModuleTextures::CleanUpTextures()
//{
//	LOG("Freeing textures and Image library");
//
//	ListItem<SDL_Texture*>* item = textures.start;
//
//	while (item != NULL)
//	{
//		SDL_DestroyTexture(item->data);
//		item = item->next;
//	}
//
//	textures.clear();
//
//	texturePath.clear();
//
//	return true;
//}

//Load new texture from file path
SDL_Texture* ModuleTextures::Load(std::string path, bool isName)
{
	if(isName)
	{
		path = config.child(path.c_str()).attribute("path").as_string();
	}

	for (int i = 0, count = texturePath.count(); i < count; i++)
	{
		if (texturePath[i].path == path)
		{
			return textures[texturePath[i].index];
		}
	}

	SDL_Texture* texture = nullptr;

	SDL_Surface* surface = app->assetManager->Load_physfs_texture(path.c_str());

	if(surface == nullptr)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(app->renderer->renderer, surface);

		if(texture == nullptr)
		{
			LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			textures.add(texture);
			TexturePath tp = { path, textures.count() - 1, (int)&texture};
			texturePath.add(tp);
			//texturePath.insert(std::pair<std::string, int>(path, textures.count() - 1));
			//printf("direccion in memory : %#x || path: %s \n", texture, path.c_str());
		}

		SDL_FreeSurface(surface);
	}
	return texture;
}

// Free texture from memory
void ModuleTextures::Unload(SDL_Texture* texture)
{
	ListItem<SDL_Texture*>* item = textures.start;

	while(item != NULL)
	{
		if(item->data == texture)
		{
			SDL_DestroyTexture(item->data);
			textures.del(item);
			break;
		}
		item = item->next;
	}
}

void ModuleTextures::SetGauss(SDL_Texture* texture, float sigma, int size)
{
	// Reference

	// https://bacprogramming.wordpress.com/2018/01/10/box-blur-with-sdl2/

	// https://stackoverflow.com/questions/19107476/get-pixel-info-from-sdl2-texture

	string path;

	// get path
	for (int i = 0; i < textures.count(); i++)
	{
		if (textures[i] == texture)
		{
			path = texturePath[i].path;
			break;
		}
		if (i = textures.count() - 1) return;
	}

	int halfSize = (int)(size / 2);

	SDL_Surface* surface = app->assetManager->Load_physfs_texture(path.c_str());
	//SDL_Surface* surface = IMG_Load(path.c_str());

	vector<vector<float>> gauss(size, vector<float>(size));

	float totalPower = 0;

	// Init gauss matrix with sigma
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			gauss[i][j] = 1 / (2 * PI * pow(sigma, 2)) * pow(N_E, -(pow(i - halfSize, 2) + pow(j - halfSize, 2)) / (2 * pow(sigma, 2)));

			// WHEN FINISHED LOOP, totalPower MOST BE 1 OR APPROACH TO 1
			totalPower += gauss[i][j];
		}
	}

	// Print gauss matrix
	//for (int i = 0; i < size; i++)
	//{
	//	for (int j = 0; j < size; j++)
	//	{
	//		printf("X:%d Y:%d :", i - halfSize, j - halfSize);
	//		printf("%f\t", gauss[i][j]);
	//	}
	//	printf("\n");
	//}
	
	for (int y = 0; y < surface->h; y++)
	{
		for (int x = 0; x < surface->w; x++)
		{
			// result
			Uint8 resultR = 0, resultG = 0, resultB = 0, resultA = 0;
			
			// temporal variable
			Uint8 r = 0, g = 0, b = 0, a = 0;

			// Get color in surface
			Uint32 color1 = ((Uint32*)surface->pixels)[y * surface->w + x];

			// Get separated r g b a 
			SDL_GetRGBA(color1, surface->format, &r, &g, &b, &a);

			// if alpha in this pixel is 0, go to the next
			if (a == 0) continue;

			// Gaussian algorithm
			for (int i = -halfSize; i < halfSize; i++)
			{
				for (int j = -halfSize; j < halfSize; j++)
				{
					if ((y + i) < 0 || (x + j) < 0 || (y + i) > surface->h || (x + j) > surface->w) continue;

					Uint32 color = ((Uint32*)surface->pixels)[(y + i) * surface->w + x + j];

					SDL_GetRGBA(color, surface->format, &r, &g, &b, &a);

					resultR += r * gauss[i + halfSize][j + halfSize];
					resultG += g * gauss[i + halfSize][j + halfSize];
					resultB += b * gauss[i + halfSize][j + halfSize];
					//result.a += a * gauss[i + halfSize][j + halfSize];
				}
			}

			// Save result in Unint32
			Uint32 color2 = (resultR) | (resultG << 8) | (resultB << 16) | (a << 24);

			// modify surface current pixel 
			((Uint32*)surface->pixels)[(y * surface->w + x)] = color2;
		}
	}

	// Update texture with new surface
	SDL_UpdateTexture(texture, nullptr, surface->pixels, surface->pitch);

	// Free memory
	SDL_FreeSurface(surface);
}

void ModuleTextures::TestEffect()
{
	int w = app->window->width;
	int h = app->window->height;

	SDL_Surface* surface = SDL_CreateRGBSurface(0, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	SDL_RenderReadPixels(app->renderer->renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);

	bool draw = true;

	for (int y = 0; y < surface->h; y++)
	{
		for (int x = 0; x < surface->w; x++, draw = !draw)
		{
			// temporal variable
			Uint8 r = 0, g = 0, b = 0, a = 0;

			// Get color in surface
			Uint32 color1 = ((Uint32*)surface->pixels)[(y * surface->w / 2) + x];

			// Get separated r g b a 
			SDL_GetRGBA(color1, surface->format, &r, &g, &b, &a);

			if (draw)
			{
				r = 155;
				g = 0;
				b = 0;
				a = 50;
			}
			else
			{
				a = 0;
			}
			
			// Save result in Unint32
			Uint32 color2 = (r) | (g << 8) | (b << 16) | (a << 24);

			// modify surface current pixel 
			((Uint32*)surface->pixels)[(y * surface->w / 2) + x] = color2;
		}
		//if (y % 2 == 0) draw = !hhdraw
	}

	if (screenTex == nullptr) screenTex = SDL_CreateTextureFromSurface(app->renderer->renderer, surface);
	
	else SDL_UpdateTexture(screenTex, nullptr, surface->pixels, surface->pitch);

	RenderObject screen;

	screen.InitAsTexture(screenTex, { 0,0 }, { 0,0,0,0 }, 1, 4, 300);

	app->renderer->AddRenderObjectRenderQueue(screen);

	// Free memory
	SDL_FreeSurface(surface);
}
