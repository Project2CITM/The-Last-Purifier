#include "GUIButton.h"
#include "Application.h"

GUIButton::GUIButton(Application* app, iPoint pos, int width, int height, std::string path) : GUI(app)
{
	InitAsBox(pos.x, pos.y, width, height);

	renderObject = new RenderObject();
	renderObject->texture = _app->textures->Load(path);
	renderObject->destRect = { pos.x, pos.y, width, height };
	renderObject->layer = 3;
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

GUIButton::GUIButton(Application* app, iPoint pos, int width, int height) : GUI(app)
{
	InitAsBox(pos.x, pos.y, width, height);
}

GUIButton::~GUIButton()
{
	RELEASE(renderObject);
}

void GUIButton::Update()
{
	if (CheckOnMouse() && (_app->input->GetMouseButton(1) == KEY_DOWN))
	{
		lastState = buttonState;
		buttonState = ButtonState::PRESS_DOWN;
		isPressed = true;

		// Sound Effect
		if (lastState != buttonState && !navigation) _app->audio->PlayFx(SFX::BUTTONDOWN);
	}
	else if (isPressed && _app->input->GetMouseButton(1) == KEY_REPEAT)
	{
		lastState = buttonState;
		buttonState = ButtonState::PRESSED;
	}
	else if (CheckOnMouse())
	{
		lastState = buttonState;

		buttonState = ButtonState::FOCUS;

		// Sound Effect
		if (lastState != buttonState && !navigation) _app->audio->PlayFx(SFX::BUTTONFEEDBACK);
	}

	if (_app->input->GetMouseButton(1) == KEY_UP)
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
			defaultColor = { 255, 255, 255, 255};
		break;

		case ButtonState::FOCUS:
			defaultColor = { 100, 200, 100, 255};
			break;

		case ButtonState::PRESS_DOWN:
		case ButtonState::PRESSED:
			defaultColor = { 100, 0, 0, 255};
			break;
	}
}

void GUIButton::PostUpdate()
{
	if (renderObject != nullptr && renderObject->texture != nullptr)
	{
		int newBtnState = CLAMP((int)buttonState, 0, 2);

		_app->renderer->AddTextureRenderQueue(renderObject->texture, { position.x, position.y }, renderSections[newBtnState], renderObject->scale,
											  renderObject->layer, renderObject->orderInLayer, 0, SDL_FLIP_NONE, 0);

		//if(_app->debug->debugViewGUIBounds)
		//_app->renderer->AddRectRenderQueue(SDL_Rect{ position.x,position.y,boxShape.w,boxShape.h }, defaultColor.r, defaultColor.g, defaultColor.b, 150, 3, 100, true, 0);
	}
	else
	{
		_app->renderer->AddRectRenderQueue(SDL_Rect{ position.x,position.y,boxShape.w,boxShape.h }, SDL_Color{ defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a }, true, 3, 10, 0);
	}
}
