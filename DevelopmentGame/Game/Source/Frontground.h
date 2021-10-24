#ifndef __FRONTGROUND_H__
#define __FRONTGROUND_H__

#include "Module.h"

class Frontground : public Module
{
public:
	Frontground();

	virtual ~Frontground();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	bool FadeToBlack();

	bool FadeFromBlack();

private:
	SDL_Rect r;
	Uint8 a;

	bool go_black = false;
	bool return_black = false;
	int fade_speed = 2;
};

#endif
