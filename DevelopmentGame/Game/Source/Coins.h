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

	float x, y;

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

	void PickCoin(float x, float y);

	int ncoins = 0;
	char numCoins[4] = { "\0" };

private:

	SDL_Texture* texture = NULL;
	Animation* currentAnimation = NULL;

	Animation idleAnim;

	int w = 16, h = 16;

	DynArray<Coin> coins;

};

#endif