#include "Application.h"
#include <iostream>

Application::Application()
{
	renderer = new ModuleRender(this);
	window = new ModuleWindow(this);
	textures = new ModuleTextures(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	physics = new ModulePhysics(this);
	scene = new ModuleScene(this);
	ui = new ModuleUI(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(input);
	AddModule(physics);

	// Game Logic
	AddModule(scene);

	AddModule(textures);
	
	AddModule(audio);
	AddModule(ui);
	
	//Render
	AddModule(renderer);
}

Application::~Application()
{
}

bool Application::Init()
{
	bool ret = true;

	// L01: DONE 3: Load config from XML
	config = LoadConfig(configF);
	LoadSaveFile();

	if (config.empty() == false)
	{
		ret = true;
		configApp = config.child("app");

		// L01: DONE 4: Read the title from the config file
		title = configApp.child("title").child_value();
		organization = configApp.child("organization").child_value();
	}

	// Call Init() in all modules
	ListItem<Module*>* item = list_modules.start;

	while(item != NULL && ret == true)
	{
		pugi::xml_node n = config.child(item->data->name.c_str());
		ret = item->data->Init(n);
	
		item = item->next;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.start;

	while(item != NULL && ret == true)
	{
		if(item->data->IsEnabled())
			ret = item->data->Start();
		item = item->next;
	}

	LoadGameRequest();

	return ret;
}

// Call PreUpdate, Update and PostUpdate on all modules
UpdateStatus Application::Update()
{
	if (isExiting) return UPDATE_STOP;
	UpdateStatus ret = UPDATE_CONTINUE;
	ListItem<Module*>* item = list_modules.start;

	while (item != NULL && ret == UPDATE_CONTINUE)
	{
		if (item->data->IsEnabled())
			ret = item->data->PreUpdate();
		item = item->next;
	}

	item = list_modules.start;

	while (item != NULL && ret == UPDATE_CONTINUE)
	{
		if (item->data->IsEnabled())
			ret = item->data->Update();
		item = item->next;
	}

	item = list_modules.start;

	while (item != NULL && ret == UPDATE_CONTINUE)
	{
		if (item->data->IsEnabled())
			ret = item->data->PostUpdate();
		item = item->next;
	}

	item = list_modules.start;

	while (item != NULL && ret == UPDATE_CONTINUE)
	{
		if (item->data->IsEnabled())
			ret = item->data->EndUpdate();
		item = item->next;
	}

	//// L02: DONE 1: This is a good place to call Load / Save methods
	if (saveGameRequested == true) SaveGame();
	if (loadGameRequested == true) LoadGame();

	// Update globalTime
	globalTime.Update();

	deltaTime = globalTime.getDeltaTime();

	if ((deltaTime <= frameTime) && vsync)
	{
		//printf("B: %f\n", deltaTime);
		sleepTime = (frameTime - deltaTime) * 1000;
		Sleep(sleepTime);
	}
	
	// Update info for title
	UpdateTitle();

	// Resete timer
	globalTime.Reset();

	// Test func
	//ShowTime();

	return ret;
}

void Application::ShowTime()
{
	//OPTICK_EVENT();
	LOG("Finish Update");
}

bool Application::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item = list_modules.end;

	while(item != nullptr && ret == true)
	{
		try
		{
			ret = item->data->CleanUp();
			item = item->prev;
		}
		catch(const exception& e)
		{
			LOG(e.what());
			LOG("ERROOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOR");
		}
	}

	list_modules.clearPtr();

	return ret;
}

pugi::xml_node Application::LoadConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description())
	else ret = configFile.child("config");

	return ret;
}

bool Application::LoadSaveFile() 
{
	pugi::xml_parse_result result = saveF.load_file(SAVE_STATE_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", SAVE_STATE_FILENAME, result.description())
	else return true;

	return false;
}

void Application::ExitGame()
{
	isExiting = true;
}

void Application::AddModule(Module* mod)
{
	list_modules.add(mod);
}

void Application::LoadGameRequest()
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist
	loadGameRequested = true;
}

// ---------------------------------------
void Application::SaveGameRequest() const
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist and... should we overwriten
	saveGameRequested = true;
}

// ---------------------------------------
const char* Application::GetTitle() const
{
	return title.c_str();
}

// ---------------------------------------
const char* Application::GetOrganization() const
{
	return organization.c_str();
}

// ---------------------------------------
// then call all the modules to load themselves
bool Application::LoadGame()
{
	bool ret = false;

	ListItem<Module*>* item = list_modules.start;

	while (item != nullptr)
	{
		item->data->LoadSaveData(saveF);
		item = item->next;
	}

	loadGameRequested = false;

	return ret;
}

// L02: TODO 7: Implement the xml save method for current state
bool Application::SaveGame()
{
	bool ret = true;

	/*ListItem<Module*>* item = list_modules.start;

	while (item != nullptr)
	{
		item->data->GetSaveData(saveF);
		item = item->next;
	}*/

	saveF.save_file(SAVE_STATE_FILENAME);

	saveGameRequested = false;

	return ret;
}

void Application::UpdateTitle()
{
	// Update globalTime
	globalTime.Update();

	dt = globalTime.getDeltaTime();

	double fps = 1 / dt;

	averageFps = (averageFps + fps) / 2;

	string printTitle;

	string vsyncMessage = vsync ? "true" : "false";

	/*printTitle = title + " | FPS: " + to_string(fps) +
		" | Avg.FPS: " + to_string(averageFps) +
		" | Last dt: " + to_string(dt * 1000) +
		"ms | Vsync: " + vsyncMessage;

	// Print new title
	window->SetTitle(printTitle.c_str());*/
}
