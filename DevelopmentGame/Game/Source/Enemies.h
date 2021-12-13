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

private:
	void PreUpdate();

	void Update(float dt);

	void Draw();

	void CreateGroundEnemy(float x, float y);
	void ReviveGroundEnemy(Enemy* enemy);

	void MoveGroundEnemy(Enemy* enemy, float dt);
	void CheckPlayer(Enemy* enemy);

	void EnemyHunting(Enemy* enemy, float dt);
	void EnemyReturning(Enemy* enemy, float dt);

	Animation slime_walkAnimR;
	Animation slime_walkAnimL;

public:
	float origin_x, origin_y;
	int w = 16, h = 16;
	float speed;

	Animation* currentAnimation = NULL;

	bool lookLeft;

	float detectionRange;
	bool enemy_spoted;

	ENEMY_STATE state;

	// only used in ground enemies
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

private:
	void PreUpdate();

	void Update(float dt);

	void Draw();

	DynArray<Enemy> enemies;

	void CreateAirEnemy(float x, float y);
	void ReviveAirEnemy(Enemy* enemy);

	void MoveAirEnemy(Enemy* enemy, float dt);
	void CheckAirEnemy(Enemy* enemy, float dt);
	void CheckPlayer(Enemy* enemy);

	void EnemyHunting(Enemy* enemy, float dt);
	void EnemyReturning(Enemy* enemy, float dt);
	
	Animation floper_walkAnim;

public:
	float origin_x, origin_y;
	int w = 16, h = 16;
	float speed;

	Animation* currentAnimation = NULL;

	bool lookLeft;

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
