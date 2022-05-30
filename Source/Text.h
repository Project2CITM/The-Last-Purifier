#ifndef __TEXT_H__
#define __TEXT_H__

#include "Point.h"
#include <string>

#define LETTER_NUMS 67

class Text
{
public:

	Text(iPoint position, std::string text = "hello" , std::string font = "defaultFont", bool printOnScene = true);

	~Text();

	void PreUpdate();

	void Update();

	void PostUpdate();

	void SetText(std::string text);

	void SetColor(SDL_Color color);

	void ChangeDrawMode();

private:
	void DrawTextStepToStep();

	void SaveTextInAscii();

public:

	iPoint position = { 0,0 };

	bool pendingToDelate = false;

	bool printOnScene = true;

	RenderObject textRO;

private:

	std::string text = "hello";

	int textToAscii[LETTER_NUMS] = { 65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89, 90, // A-Z
		97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122, // a-z
		48,49,50,51,52,53,54,55,56,57, // 0-9
		44,46,40,41,32 }; // , . ( ) SPACE

	List<int> textInCode;

	int spaceInLetter = 0;

	Application* app = nullptr;

	float currentTextCountTime;

	float textCountTime = 0.0f;
};

#endif // !__TEXT_H__