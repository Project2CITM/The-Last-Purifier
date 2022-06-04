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

	//costText = new Text(position, "");
	//costText->setLayer(layer);
	//costText->setOrderInLayer(orderInlayer + 1.0f);
}

GUIHoverBox::~GUIHoverBox()
{

}

void GUIHoverBox::Update()
{
	//if (!bttn->buttonState == ButtonState::FOCUS) return;
	
	iPoint mousePos = { app->input->GetMouseX() / (int) app->window->scale, app->input->GetMouseY() / (int) app->window->scale};

	this->position = mousePos;
	
	//Visual Enhancment on top of the mouse
	position.x = position.x + 2;
	position.y = position.y - boxShape.h;

	text->SetText("Increases health by 0.2");
	text->position = position;

	//costText->position = { position.x, position.y + 11 };

}

void GUIHoverBox::PostUpdate()
{
	//if (!bttn->buttonState == ButtonState::FOCUS) return;

	app->renderer->AddRectRenderQueue(SDL_Rect{ position.x,position.y,boxShape.w,boxShape.h }, renderColour, true, layer, orderInlayer, 0);
	boxShape.h = 11;
}

void GUIHoverBox::SetData(std::string txt, int cost)
{
	//Basic text
	text->SetText(txt);

	////Cost text
	//std::string txtCost = "";
	//if (cost > 0)
	//{
	//	txtCost = std::to_string(cost);
	//	txtCost += " Souls";

	//	//Increases size of box
	//	boxShape.h += 11;
	//}
	//costText->SetText(txtCost);
	

	//Gives width for all characters with a bit of margin at the end
	boxShape.w = (txt.length() * 7) + 4;
	//Arranges text (depending of the size of the string) to the right
	//costText->position.x = (costText->position.x + boxShape.w) - (txtCost.length() * 7) - 4;
}
