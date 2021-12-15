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

	virtual void PreUpdate();
	virtual void HandleInput(float dt);
	virtual void Update(float dt);
	virtual void Draw();

	virtual void DeleteEntity();

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

	void CreateEntity(ENTITY_TYPE entity_type, float x, float y);
	void DestroyEntity(Entity* entity);

	List<Entity*> entities;

	void KillEnemy(fPoint pos);

	SDL_Texture* slime_textureR = NULL;
	SDL_Texture* slime_textureL = NULL;
	SDL_Texture* floper_texture = NULL;
	
};

#endif