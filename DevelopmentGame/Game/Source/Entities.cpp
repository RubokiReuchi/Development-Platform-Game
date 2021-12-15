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
		entities.At(i)->data->PreUpdate();
	}

	return true;
}

// Called each loop iteration
bool Entities::Update(float dt)
{
	for (size_t i = 0; i < entities.Count(); i++)
	{
		entities.At(i)->data->HandleInput(dt);
		entities.At(i)->data->Update(dt);
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
			entities.At(i)->data->Draw();
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

void Entities::CreateEntity(ENTITY_TYPE entity_type, float x, float y)
{
	Entity* en = new Entity();
	switch (entity_type)
	{
	case ENTITY_TYPE::PLAYER:
		// create player
		break;
	case ENTITY_TYPE::GROUND_ENEMY:
		entities.Add(Ground_Enemies::CreateGroundEnemy(x, y));
		break;
	case ENTITY_TYPE::AIR_ENEMY:
		//entities.Add(static_cast<Air_Enemies*>(en)->CreateAirEnemy(x, y));
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
	for (size_t i = 0; i < entities.Count(); i++)
	{
		if (entities.At(i)->data->entity_type == ENTITY_TYPE::GROUND_ENEMY || entities.At(i)->data->entity_type == ENTITY_TYPE::AIR_ENEMY)
		{
			if (pos.x + 1.5f > entities.At(i)->data->position.x && pos.x - 1.5f < entities.At(i)->data->position.x && pos.y + 2.0f > entities.At(i)->data->position.y && pos.y - 2.0f < entities.At(i)->data->position.y)
			{
				entities.At(i)->data->DeleteEntity();

				break;
			}
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

void Entity::DeleteEntity()
{

}