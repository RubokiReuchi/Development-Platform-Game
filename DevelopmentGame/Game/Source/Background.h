#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include "Module.h"

class Background : public Module
{
public:
	Background();

	virtual ~Background();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	SDL_Rect r;
	SDL_Texture* texture;
};

#endif