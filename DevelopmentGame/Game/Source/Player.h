#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Physics.h"

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

private:
	float x, y;
	int w = 32, h = 32;
	float speed = 10.0f;
	float jumpForce = 500.0f;

	float desiredVel = 0;
};

#endif
