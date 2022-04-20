#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
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
			try
			{
				SDL_DestroyTexture(textures[i]);
				textures[i] = nullptr;
			}
			catch (const exception& e)
			{
				LOG(e.what());
				LOG("ERROOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOR");
			}
		}
		LOG("Succeful clean texture");
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

	/*std::map<std::string, int>::iterator it;
	it = texturePath.find(path);*/

	for (int i = 0, count = texturePath.count(); i < count; i++)
	{
		if (texturePath[i].path == path)
		{
			return textures[texturePath[i].index];
		}
	}

	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = IMG_Load(path.c_str());

	// Testing
	SDL_Surface* surface2 = IMG_Load(path.c_str());

	// https://bacprogramming.wordpress.com/2018/01/10/box-blur-with-sdl2/

	//https://stackoverflow.com/questions/19107476/get-pixel-info-from-sdl2-texture

	/*
	* 
	for (int y = 0; y < surface2->h; y++)
	{
		for (int x = 0; x < (surface2->pitch / 4); x++)
		{
			Uint32 color = ((Uint32*)surface2->pixels)[(y * (surface2->pitch / 4)) + x];

			Uint8 r = 0, g = 0, b = 0, a = 0;

			SDL_GetRGBA(color, surface2->format, &r, &g, &b, &a);

			if (a != 0)
			{
				r = 0;
				g = 255;
				b = 0;
				a = 255;
			}

			color = (r) | (g << 8) | (b << 16) | (a << 24);

			((Uint32*)surface2->pixels)[(y * (surface2->pitch / 4)) + x] = color;
		}
	}

	*/

	if(surface == NULL)
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

		/*
		SDL_UpdateTexture(texture, nullptr, surface2->pixels, surface2->pitch);

		SDL_FreeSurface(surface2);
		*/
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