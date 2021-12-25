#ifndef __MENU_H__
#define __MENU_H__

#include "Module.h"

#define NUM_PAUSE_BUTTONS 4
#define NUM_DEAD_BUTTONS 2

struct Button {
	SDL_Rect rect = { 0, 0, 220, 70 };
	SDL_Texture* tex = NULL;

	int state = 0; // 0->idle, 1->above
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

	bool GetGameState();

	bool dead;

	bool lose;


private:
	SDL_Rect r;
	bool paused;

	int chosed;
	Button pause_buttons[NUM_PAUSE_BUTTONS];
	Button dead_buttons[NUM_DEAD_BUTTONS];

	SDL_Texture* gameOver = NULL;
	SDL_Texture* cat = NULL;

	Uint8 idleColorR = 18;
	Uint8 idleColorG = 188;
	Uint8 idleColorB = 13;
	Uint8 inColorR = 255;
	Uint8 inColorG = 178;
	Uint8 inColorB = 0;
	Uint8 pColorR = 255;
	Uint8 pColorG = 0;
	Uint8 pColorB = 0;

	int load_cd = 120;
	bool loading = false;

	uint win_w, win_h;
	
};

#endif