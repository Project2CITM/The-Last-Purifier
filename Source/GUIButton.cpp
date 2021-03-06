#include "GUIButton.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleTextures.h"

GUIButton::GUIButton(iPoint pos, int width, int height, MenuButton currentMenu, SDL_Texture* texture) : GUI()
{
	InitAsBox(pos.x, pos.y, width, height, currentMenu);

	renderObject = new RenderObject();
	renderObject->texture = texture;
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

	Hover = app->audio->LoadFx("Assets/Audio/SFX/UI/sfx_uiHover.wav", false);
	Press = app->audio->LoadFx("Assets/Audio/SFX/UI/sfx_uiSelect.wav", false);
}

GUIButton::GUIButton(iPoint pos, int width, int height, MenuButton currentMenu) : GUI()
{
	InitAsBox(pos.x, pos.y, width, height, currentMenu);
}

GUIButton::GUIButton(iPoint pos, int width, int height, SDL_Texture* texture) : GUI()
{
	InitAsBox(pos.x, pos.y, width, height);

	renderObject = new RenderObject();
	renderObject->texture = texture;
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

	Hover = app->audio->LoadFx("Audio/SFX/UI/sfx_uiHover.wav");
	Press = app->audio->LoadFx("Audio/SFX/UI/sfx_uiSelect.wav");
}

GUIButton::~GUIButton()
{
	RELEASE(renderObject);
}

void GUIButton::Update()
{
	if (!app->input->usingGameController) UpdateMouse();
	else if (!isHoverBox)
	{
		lastState = buttonState;
		buttonState = ButtonState::IDLE;
	}

	if ((buttonState == ButtonState::FOCUS) && isHoverBox)
	{
		HoverBox.Update();
		lastState = buttonState;
		return;
	}
	else if (buttonState != ButtonState::FOCUS && isHoverBox)
	{
		HoverBox.SetData("");
	}
}

void GUIButton::PostUpdate()
{
	if (renderObject != nullptr && renderObject->texture != nullptr)
	{
		int newBtnState = CLAMP((int)buttonState, 0, 2);

		app->renderer->AddTextureRenderQueue(renderObject->texture, { position.x, position.y }, renderSections[newBtnState], renderObject->scale,
											  renderObject->layer, renderObject->orderInLayer, 0, SDL_FLIP_NONE, 0);

		//if(app->debug->debugViewGUIBounds)
		//app->renderer->AddRectRenderQueue(SDL_Rect{ position.x,position.y,boxShape.w,boxShape.h }, defaultColor.r, defaultColor.g, defaultColor.b, 150, 3, 100, true, 0);
	}
	else
	{
		app->renderer->AddRectRenderQueue(SDL_Rect{ position.x,position.y,boxShape.w,boxShape.h }, renderColour, true, layer, orderInlayer, 0);
	}

	if (buttonState == ButtonState::FOCUS && isHoverBox)
	{
		HoverBox.PostUpdate();
	}
}

void GUIButton::PressButton(bool doAction)
{
	lastState = buttonState;
	buttonState = ButtonState::PRESS_DOWN;
	isPressed = true;

	this->doAction = doAction;

	if (app->input->GetMouseButton(1) == KEY_UP) return;

	// Sound Effect
	if (lastState != buttonState && !navigation) app->audio->PlayFx(Press);
}

void GUIButton::HoverButton()
{
	if (buttonState == ButtonState::FOCUS) return;
	buttonState = ButtonState::FOCUS;
	if (lastState != buttonState && !navigation) app->audio->PlayFx(Hover);
}

void GUIButton::UpdateMouse()
{
	if (CheckOnMouse() && (app->input->GetMouseButton(1) == KEY_DOWN))
	{
		PressButton();
	}
	else if (isPressed && app->input->GetMouseButton(1) == KEY_REPEAT)
	{
		lastState = buttonState;
		buttonState = ButtonState::PRESSED;
	}
	else if (CheckOnMouse())
	{
		lastState = buttonState;

		buttonState = ButtonState::FOCUS;

		//if (buttonState == ButtonState::FOCUS)
			//navigation = true;

		// Sound Effect
		if (lastState != buttonState && !navigation) app->audio->PlayFx(Hover);
	}

	if (app->input->GetMouseButton(1) == KEY_UP)
	{
		// We do the action
		if (isPressed && CheckOnMouse())
		{
			doAction = true;
		}
		isPressed = false;
		buttonState = ButtonState::IDLE;
	}
	else if (!CheckOnMouse())
	{
		if (!navigation || !isPressed)
		{
			buttonState = ButtonState::IDLE;
			isPressed = false;
		}
	}

	switch (buttonState)
	{
	case ButtonState::IDLE:
		defaultColor = { 255, 255, 255, 255 };
		break;

	case ButtonState::FOCUS:
		defaultColor = { 100, 200, 100, 255 };
		break;

	case ButtonState::PRESS_DOWN:
	case ButtonState::PRESSED:
		defaultColor = { 100, 0, 0, 255 };
		break;
	}
}
