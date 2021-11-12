#ifndef __BULLETS_H__
#define __BULLETS_H__

#include "Module.h"
#include "Physics.h"
#include "Animation.h"

class Bullets : public Module
{
public:
	Bullets();

	virtual ~Bullets();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void SpawnBullet(int x, int y, int direction);

private:
	float speed = 10.0f;
	int lifeTime = 10;

	b2Body* body;
	
	SDL_Texture* tex = NULL;
	Animation* currentAnimation = NULL;

	Animation flyAnimR;
	Animation flyAnimL;
	Animation exploteAnim;

};

#endif