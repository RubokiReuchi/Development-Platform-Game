#ifndef __COINS_H__
#define __COINS_H__

#include "Module.h"
#include "Physics.h"
#include "Animation.h"
#include "DynArray.h"

struct SDL_Texture;

struct Coin
{
	b2Body* body;

	int x, y;

	bool picked;
};

class Coins : public Module
{
public:

	Coins();

	// Destructor
	virtual ~Coins();

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

	void CreateCoin(int x, int y);

private:

	SDL_Texture* texture = NULL;
	Animation* currentAnimation = NULL;

	Animation idleAnim;

	int w, h = 32;

	DynArray<Coin> coins;

};

#endif