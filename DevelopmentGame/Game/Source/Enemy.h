#ifndef __ENEMIES_H__
#define __ENEMIES_H__

#include "Module.h"
#include "Physics.h"
#include "Animation.h"
#include "DynArray"

struct Enemy {
	b2Body* body;

	float x, y;
	int w = 32, h = 32;
	float speed;

	Animation* currentAnimation = NULL;

	bool lookLeft;

	float detectionRange;
};

class Enemies : public Module
{
public:
	Enemies();

	virtual ~Enemies();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

	SDL_Texture* textureR = NULL;
	SDL_Texture* textureL = NULL;

	DynArray<Enemy> enemies;

	void CreateGroundEnemy(float x, float y);
	void CreateAirEnemy(float x, float y);

};

#endif
