#ifndef __MENU_H__
#define __MENU_H__

#include "Module.h"

#define NUM_BUTTONS 4

struct Button {
	SDL_Rect rect = { 0, 0, 200, 50 };
	SDL_Texture* tex = NULL;

	int state = 0; // 0->idle, 1->above, 2->clicked
};

class Menu : public Module
{
public:
	Menu();

	virtual ~Menu();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

private:
	SDL_Rect r;
	bool paused;

	int chosed;
	Button buttons[NUM_BUTTONS];

	Uint8 idleColorR = 18;
	Uint8 idleColorG = 188;
	Uint8 idleColorB = 13;
	Uint8 inColorR = 8;
	Uint8 inColorG = 255;
	Uint8 inColorB = 0;
	Uint8 clickedColorR = 255;
	Uint8 clickedColorG = 178;
	Uint8 clickedColorB = 0;
	
};

#endif