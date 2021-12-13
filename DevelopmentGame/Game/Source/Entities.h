#ifndef __ENTITIES_H__
#define __ENTITIES_H__

#include "Module.h"
#include "Physics.h"
#include "Animation.h"
#include "DynArray.h"
#include "Point.h"

enum class ENTITY_TYPE
{
	PLAYER,
	GROUND_ENEMY,
	AIR_ENEMY
};

class Entity
{
public:
	Entity() {}
	~Entity() {}

	virtual void PreUpdate();
	virtual void HandleInput(float dt);
	virtual void Update(float dt);
	virtual void Draw();

public:
	ENTITY_TYPE entity_type;

	fPoint position;

	b2Body* body;
};

class Entities : public Module
{
public:
	Entities();

	virtual ~Entities();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

	Entity* CreateEntity(ENTITY_TYPE entity_type);
	void DestroyEntity(Entity* entity);

	DynArray<Entity> entities;

	void KillEnemy(fPoint pos);
	
};

#endif