#ifndef __ENEMIES_H__
#define __ENEMIES_H__

#include "Module.h"
#include "Entities.h"
#include "Physics.h"
#include "Animation.h"
#include "DynArray.h"

enum class ENEMY_STATE
{
	IDLE,
	HUNT,
	RETURN,
	DEATH
};

class Ground_Enemies : public Entity
{
public:
	Ground_Enemies();

	virtual ~Ground_Enemies();

	void CreateGroundEnemy(float x, float y);

private:
	void PreUpdate();

	void Update(float dt);

	void Draw();

	void ReviveGroundEnemy();

	void MoveGroundEnemy(float dt);
	void CheckPlayer();

	void EnemyHunting(float dt);
	void EnemyReturning(float dt);

	Animation slime_walkAnimR;
	Animation slime_walkAnimL;

public:
	float origin_x, origin_y;
	int w = 16, h = 16;
	float speed;

	Animation* currentAnimation = NULL;

	float detectionRange;
	bool enemy_spoted;

	ENEMY_STATE state;

	// only used in ground enemies
	bool lookLeft;
	float idleOb_x;
	bool obLeft;
	//

	bool plan_to_delete = false;

	PathFinding* path_save;
};

class Air_Enemies : public Entity
{
public:
	Air_Enemies();

	virtual ~Air_Enemies();

	void CreateAirEnemy(float x, float y);

private:
	void PreUpdate();

	void Update(float dt);

	void Draw();

	void ReviveAirEnemy();

	void MoveAirEnemy(float dt);
	void CheckAirEnemy(float dt);
	void CheckPlayer();

	void EnemyHunting(float dt);
	void EnemyReturning(float dt);
	
	Animation floper_walkAnim;

public:
	float origin_x, origin_y;
	int w = 16, h = 16;
	float speed;

	Animation* currentAnimation = NULL;

	float detectionRange;
	bool enemy_spoted;

	ENEMY_STATE state;

	// only used in air enemies
	int cd_air_enemy = 0;
	//

	bool plan_to_delete = false;

	PathFinding* path_save;
};

#endif
