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
	void InitCustomEntity();

	bool PreUpdate();

	bool Update(float dt);

	bool Draw();

	void ReviveGroundEnemy();

	void MoveGroundEnemy(float dt);
	void CheckPlayer();

	void EnemyHunting(float dt);
	void EnemyReturning(float dt);

	bool DeleteEntity();

	// load-save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);

	void SwitchDirection();

public:
	float origin_x, origin_y;
	int w = 16, h = 16;
	float speed;

	Animation* currentAnimation = NULL;

	float detectionRange;
	bool enemy_spoted;

	ENEMY_STATE state;

	bool lookLeft;
	float idleOb_x;
	bool obLeft;

	bool plan_to_delete = false;

	PathFinding* path_save = NULL;

	Animation slime_walkAnimR;
	Animation slime_walkAnimL;
};

class Air_Enemies : public Entity
{
public:
	Air_Enemies();

	virtual ~Air_Enemies();

private:
	void InitCustomEntity();

	bool PreUpdate();

	bool Update(float dt);

	bool Draw();

	void ReviveAirEnemy();

	void MoveAirEnemy(float dt);
	void CheckAirEnemy(float dt);
	void CheckPlayer();

	void EnemyHunting(float dt);
	void EnemyReturning(float dt);

	bool DeleteEntity();

	// load-save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);

	// load-save
	bool Load();
	bool Save();

public:
	float origin_x, origin_y;
	int w = 16, h = 16;
	float speed;

	Animation* currentAnimation = NULL;

	float detectionRange;
	bool enemy_spoted;

	ENEMY_STATE state;

	float cd_air_enemy = 0;

	bool plan_to_delete = false;

	PathFinding* path_save = NULL;

	Animation floper_walkAnim;
};

#endif
