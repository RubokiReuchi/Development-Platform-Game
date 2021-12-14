#include "App.h"
#include "Textures.h"
#include "Entities.h"
#include "Scene.h"

#include "Enemies.h"

#include "Defs.h"
#include "Log.h"

#include <time.h>

Entities::Entities() : Module()
{
	name.Create("entities");
	
}

// Destructor
Entities::~Entities()
{}

// Called before render is available
bool Entities::Awake()
{
	srand(time(NULL));

	return true;
}

// Called before the first frame
bool Entities::Start()
{
	// enemies textures
	slime_textureR = app->tex->Load("Assets/textures/SlimeR.png");
	slime_textureL = app->tex->Load("Assets/textures/SlimeL.png");
	floper_texture = app->tex->Load("Assets/textures/Floper.png");

	return true;
}

// Called each loop iteration
bool Entities::PreUpdate()
{
	for (size_t i = 0; i < entities.Count(); i++)
	{
		entities.At(i)->PreUpdate();
	}

	return true;
}

// Called each loop iteration
bool Entities::Update(float dt)
{
	for (size_t i = 0; i < entities.Count(); i++)
	{
		entities.At(i)->HandleInput(dt);
		entities.At(i)->Update(dt);
	}

	return true;
}

// Called each loop iteration
bool Entities::PostUpdate()
{
	for (size_t i = 0; i < entities.Count(); i++)
	{
		if (!app->scene->GetStartScreenState())
		{
			entities.At(i)->Draw();
		}
	}

	return true;
}

// Called before quitting
bool Entities::CleanUp()
{

	return true;
}

bool Entities::LoadState(pugi::xml_node& data)
{
	for (size_t i = 0; i < data.attribute("value").as_int(); i++)
	{
		
	}

	return true;
}

bool Entities::SaveState(pugi::xml_node& data)
{
	for (size_t i = 0; i < entities.Count(); i++)
	{
		
	}

	return true;
}

Entity* Entities::CreateEntity(ENTITY_TYPE entity_type, float x, float y)
{
	switch (entity_type)
	{
	case ENTITY_TYPE::PLAYER:
		// create player
		break;
	case ENTITY_TYPE::GROUND_ENEMY:
		Ground_Enemies::CreateGroundEnemy(x, y);
		break;
	case ENTITY_TYPE::AIR_ENEMY:
		Air_Enemies::CreateAirEnemy(x, y);
		break;
	default:
		break;
	}
}

void Entities::DestroyEntity(Entity* entity)
{
	entity->~Entity();
}

void Entities::KillEnemy(fPoint pos)
{
	for (size_t i = 0; i < enemies.Count(); i++)
	{
		if (pos.x + 1.5f > enemies.At(i)->x && pos.x - 1.5f < enemies.At(i)->x && pos.y + 2.0f > enemies.At(i)->y && pos.y - 2.0f < enemies.At(i)->y)
		{
			enemies.At(i)->state = ENEMY_STATE::DEATH;
			enemies.At(i)->plan_to_delete = true;
			app->player->player_body->ApplyForceToCenter({ 0, -23.5f * app->GetDT() }, true);

			break;
		}
	}
}

void Entity::PreUpdate()
{

}

void Entity::HandleInput(float dt)
{

}

void Entity::Update(float dt)
{

}

void Entity::Draw()
{

}