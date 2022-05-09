#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GUI.h"
#include "string"

class GUIButton;
class RenderObject;

class GUISlider :public GUI
{
private:
	RenderObject* renderObject = nullptr;

	SDL_Rect renderSections[2];

	GUIButton* btn = nullptr;

	float value = 0;

	float max_value = 100;

	float min_value = 0;

	int btnMouse_offset_x = 0;

public:
	GUISlider(iPoint pos, int width, int height, MenuButton currentMenu, SDL_Texture* texture);

	GUISlider(iPoint pos, int width, int height, MenuButton currentMenu);

	~GUISlider();

	void CreateDefaultBtn(MenuButton currentMenu);

	void CreateGUIBtn(GUIButton* btn);

	void Update() override;

	void PostUpdate() override;

	/// <summary>
	///
	/// </summary>
	/// <param name="value">0-1</param>
	void SetValue(float val);

	float GetValue();
};

#endif // !__GUISLIDER_H__