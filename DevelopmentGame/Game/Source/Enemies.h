#ifndef __ENEMIES_H__
#define __ENEMIES_H__

#include "Module.h"
#include "Physics.h"
#include "Animation.h"
#include "DynArray.h"

enum class ENEMY_TYPE
{
	GROUND,
	AIR
};

enum class ENEMY_STATE
{
	IDLE,
	HUNT,
	DEATH
};

struct Enemy {
	b2Body* body;

	float origin_x, origin_y;
	float x, y;
	int w = 16, h = 16;
	float speed;

	ENEMY_TYPE type;

	Animation* currentAnimation = NULL;

	bool lookLeft;

	float detectionRange;
	bool enemy_spoted;

	ENEMY_STATE state;
	float idleOb_x, idleOb_y;
	bool obLeft;
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
	void MoveAirEnemy(Enemy* enemy, float dt);

	Animation idleAnimR;
	Animation idleAnimL;
	Animation walkAnimR;
	Animation walkAnimL;
	Animation deathAnimL;
	Animation deathAnimR;

};

#endif
