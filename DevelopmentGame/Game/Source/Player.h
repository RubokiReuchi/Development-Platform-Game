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

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

	b2Body* player_body;

	bool inAir = true;
	bool djump = true;

	bool Death();
	fPoint GetPosition();
	void SetPosition(int new_x, int new_y);
	float x = 8.2f, y = 22.5f;
private:
	//float x = 8.2f, y = 22.5f;
	int w = 20, h = 32;
	float speed = 0.3f;
	float jumpForce = 23.5f;

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
	Animation deathAnimL;
	Animation deathAnimR;

	uint walk_sound;
	uint jump_sound;
	int walk_cd = 20;

	bool lookLeft;
	int death_cd = 120;

	int nlifes = 0;
	char numLifes[3] = { "\0" };

};

#endif
