#ifndef __HEARTS_H__
#define __HEARTS_H__

#include "Module.h"
#include "Physics.h"
#include "Animation.h"
#include "DynArray.h"

struct SDL_Texture;

struct Heart
{
	b2Body* body;

	float x, y;

	bool picked;

	bool plan_to_delete = false;
};

class Hearts : public Module
{
public:

	Hearts();

	// Destructor
	virtual ~Hearts();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void CreateHeart(int x, int y);

	void PickHeart(float x, float y);

private:

	SDL_Texture* texture = NULL;

	int w = 16, h = 16;

	DynArray<Heart> hearts;

};

#endif