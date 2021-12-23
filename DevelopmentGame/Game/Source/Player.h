#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Entities.h"
#include "Physics.h"
#include "Animation.h"

class Player : public Entity
{
public:
	Player();

	virtual ~Player();

private:
	void InitCustomEntity();

	bool PreUpdate();

	void HandleInput(float dt);

	bool Update(float dt);

	bool Draw();

	// load-save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);

	bool inAir = true;
	bool djump = true;

	void PlayerDeath();
	fPoint GetPlayerPosition();
	void SetPlayerPosition(int new_x, int new_y);
	void ImpulsePlayer();

private:
	int w = 20, h = 32;
	float speed = 0.3f;
	float jumpForce = 25.0f;

public:
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
	int walk_cd = 320;

	bool lookLeft;
	int death_cd = 1920;

	// Hability
	Animation* habAnimation = NULL;

	Animation readyAnim;
	Animation chargeAnim;

	bool hab_ready;
	int hab_cd;
	bool dashing;
	int dash_time;

};

#endif
