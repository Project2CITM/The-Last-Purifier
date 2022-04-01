#ifndef __TEXT_H__
#define __TEXT_H__

#include "Point.h"
#include <string>

class Text
{
public:

	Text(iPoint position, std::string font = "defaultFont");

	~Text();

	void PreUpdate();

	void Update();

	void PostUpdate();

	void SetText(std::string text);

public:

	iPoint position = { 0,0 };

	bool pendingToDelate = false;
};

#endif // !__TEXT_H__