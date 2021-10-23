#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Physics.h"
#include "Animation.h"

class Player : public Module
{
public:
	Player();

	virtual ~Player();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	b2Body* player_body;

	bool inAir = true;
	bool djump = true;

private:
	float x, y;
	int w = 20, h = 32;
	float speed = 10.0f;
	float jumpForce = 500.0f;

public:
	SDL_Texture* textureR = NULL;
	SDL_Texture* textureL = NULL;
	Animation* currentAnimation = NULL;

	// A set of animations
	Animation idleAnimR;
	Animation idleAnimL;
	Animation jumpAnimR;
	Animation jumpAnimL;
	Animation fallAnimR;
	Animation fallAnimL;
	Animation walkAnimR;
	Animation walkAnimL;

	bool lookLeft;

};

#endif
