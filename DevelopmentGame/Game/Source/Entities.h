#ifndef __ENTITIES_H__
#define __ENTITIES_H__

#include "Module.h"
#include "Physics.h"
#include "Animation.h"
#include "DynArray.h"
#include "Point.h"
#include "List.h"

enum class ENTITY_TYPE
{
	PLAYER = 0,
	GROUND_ENEMY,
	AIR_ENEMY
};

class Entity
{
public:
	Entity() {}
	~Entity() {}

	void Init(ENTITY_TYPE type, fPoint p);

	virtual void InitCustomEntity();
	virtual bool PreUpdate();
	virtual void HandleInput(float dt);
	virtual bool Update(float dt);
	virtual bool Draw();
	virtual bool DeleteEntity();

	// load-save
	virtual bool Load(pugi::xml_node&);
	virtual bool Save(pugi::xml_node&);

	// custom fuctions
	virtual void SwitchDirection();

public:
	ENTITY_TYPE entity_type;

	fPoint position;

	b2Body* body;

	bool alive;

	bool init;

	int p_in_array;
};

class Entities : public Module
{
public:
	Entities();

	virtual ~Entities();

	void AddEntity(Entity* entity, ENTITY_TYPE type, fPoint p);

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

public:
	void CreateEntity(ENTITY_TYPE entity_type, float x, float y);
	void DestroyEntity(Entity* entity);

	List<Entity*> entities;

	int array_lenght = 0;

	void KillEnemy(fPoint pos);
	
};

#endif