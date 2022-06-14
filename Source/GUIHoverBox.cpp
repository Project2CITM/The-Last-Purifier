#include "GUIHoverBox.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"

#include "Text.h"

GUIHoverBox::GUIHoverBox() : GUI()
{
	InitAsBox(0, 0, 165, 10);

	app = Application::GetInstance();

	text = new Text(position, "");
	text->setLayer(layer);
	text->setOrderInLayer(orderInlayer + 1.0f);
}

GUIHoverBox::~GUIHoverBox()
{

}

void GUIHoverBox::Update()
{
	if (!controller)
	{
		float screenOffset = app->FullScreenDesktop ? (float)app->renderer->displayMode.h / 720.0f : 1;;
		iPoint mousePos = { app->input->GetMouseX() / (int)(app->window->scale * screenOffset), app->input->GetMouseY() / (int)(app->window->scale * screenOffset) };


		this->position = mousePos;

		//Visual Enhancment on top of the mouse
		position.x = position.x + 2;
		position.y = position.y - boxShape.h;
		text->position = position;
	}

}

void GUIHoverBox::PostUpdate()
{
	app->renderer->AddRectRenderQueue(SDL_Rect{ position.x,position.y,boxShape.w,boxShape.h }, renderColour, true, layer, orderInlayer, 0);
	boxShape.h = 11;
}

void GUIHoverBox::SetData(std::string txt, int cost, bool controller, iPoint position)
{
	//Basic text
	text->SetText(txt);

	//Gives width for all characters with a bit of margin at the end
	boxShape.w = (txt.length() * 7) + 4;

	if (controller)
	{
		this->position = position;
		text->position = position;
	}
	this->controller = controller;

}
