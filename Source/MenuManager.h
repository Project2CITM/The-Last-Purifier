#ifndef __MENU_MANAGER_H__
#define __MENU_MANAGER_H__

class Application;

class MenuManager
{
private:

	MenuManager();

	~MenuManager();

	static MenuManager* instance;

public:
	static MenuManager* GetInstance();

	void PreUpdate();

	void ReleaseInstance();

	bool isPause = false;

	bool canPause = true;

	bool anyMenuOn = false;

private:

	Application* app = nullptr;
};

#endif