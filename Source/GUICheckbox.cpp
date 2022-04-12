#include "GUICheckbox.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"

GUICheckbox::GUICheckbox(iPoint pos, int width, int height, MenuButton currentMenu, std::string path) : GUI()
{
	InitAsBox(pos.x, pos.y, width, height, currentMenu);

	renderObject = new RenderObject();
	renderObject->texture = app->textures->Load(path);
	renderObject->destRect = { pos.x, pos.y, width, height };
	renderObject->layer = 4;
	renderObject->orderInLayer = 15;
	renderObject->scale = 1;

	// The Button texture must be three images of the different states of the button, organized from top to bottom in order of:
	// IDLE, FOCUS, PRESSED
	for (int i = 0; i < 3; i++)
	{
		renderSections[i].x = 0;
		renderSections[i].y = i * height;
		renderSections[i].w = width;
		renderSections[i].h = height;
	}
}

GUICheckbox::GUICheckbox(iPoint pos, int width, int height, MenuButton currentMenu) : GUI()
{
	InitAsBox(pos.x, pos.y, width, height, currentMenu);
}

GUICheckbox::~GUICheckbox()
{
	RELEASE(renderObject);
}

void GUICheckbox::Update()
{
	if (CheckOnMouse() && app->input->GetMouseButton(1) == KEY_UP)
	{
		ChangeState(!isActive);
		doAction = true;
	}
	else if (CheckOnMouse() && checkboxState != CheckboxState::ON) checkboxState = CheckboxState::FOCUS;
	else if (checkboxState != CheckboxState::ON) checkboxState = CheckboxState::OFF;

	switch (checkboxState)
	{
	case CheckboxState::OFF:
		defaultColor = { 100, 0, 0, 255 };
		break;

	case CheckboxState::FOCUS:
		defaultColor = { 255, 255, 255, 255 };
		break;

	case CheckboxState::ON:
		defaultColor = { 100, 200, 100, 255 };
		break;
	}
}

void GUICheckbox::PostUpdate()
{
	if (renderObject != nullptr && renderObject->texture != nullptr)
	{
		app->renderer->AddTextureRenderQueue(renderObject->texture, { position.x, position.y }, renderSections[(int)checkboxState], renderObject->scale,
			renderObject->layer, renderObject->orderInLayer,  0, SDL_FLIP_NONE, 0);

		/*if(app->debug->debugViewGUIBounds)
		{
			app->renderer->AddRectRenderQueue(SDL_Rect{ position.x,position.y,boxShape.w,boxShape.h }, defaultColor.r, defaultColor.g, defaultColor.b, 150, 3, 100, true, 0);
		}*/
	}
}

void GUICheckbox::ChangeState(bool isOn)
{
	isActive = isOn;
	checkboxState = isActive ? CheckboxState::ON : CheckboxState::OFF;
}