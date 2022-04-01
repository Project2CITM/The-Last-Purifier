#include "Text.h"
#include "Application.h"

Text::Text(iPoint position, std::string font)
{
	Application* app = Application::GetInstance();

	std::string path = app->config.child("fonts").child(font.c_str()).attribute("path").as_string();
}

Text::~Text()
{
}

void Text::PreUpdate()
{
}

void Text::Update()
{
}

void Text::PostUpdate()
{
}

void Text::SetText(std::string text)
{
}
