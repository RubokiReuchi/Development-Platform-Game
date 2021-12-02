#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Physics.h"
#include "Player.h"
#include "Enemies.h"
#include "Menu.h"

#include "Defs.h"
#include "Log.h"

Enemies::Enemies() : Module()
{
	name.Create("enemies");
	int pix = 111;

	// idle animation
	idleAnimR.PushBack({ pix * 0, pix * 3, pix, pix });
	idleAnimR.PushBack({ pix * 1, pix * 3, pix, pix });
	idleAnimR.PushBack({ pix * 2, pix * 3, pix, pix });
	idleAnimR.PushBack({ pix * 3, pix * 3, pix, pix });
	idleAnimR.PushBack({ pix * 4, pix * 3, pix, pix });
	idleAnimR.PushBack({ pix * 5, pix * 3, pix, pix });
	idleAnimR.PushBack({ pix * 6, pix * 3, pix, pix });
	idleAnimR.PushBack({ pix * 7, pix * 3, pix, pix });
	idleAnimR.PushBack({ pix * 8, pix * 3, pix, pix });
	idleAnimR.PushBack({ pix * 9, pix * 3, pix, pix });
	idleAnimR.speed = 0.25f;

	idleAnimL.PushBack({ pix * 9, pix * 3, pix, pix });
	idleAnimL.PushBack({ pix * 8, pix * 3, pix, pix });
	idleAnimL.PushBack({ pix * 7, pix * 3, pix, pix });
	idleAnimL.PushBack({ pix * 6, pix * 3, pix, pix });
	idleAnimL.PushBack({ pix * 5, pix * 3, pix, pix });
	idleAnimL.PushBack({ pix * 4, pix * 3, pix, pix });
	idleAnimL.PushBack({ pix * 3, pix * 3, pix, pix });
	idleAnimL.PushBack({ pix * 2, pix * 3, pix, pix });
	idleAnimL.PushBack({ pix * 1, pix * 3, pix, pix });
	idleAnimL.PushBack({ pix * 0, pix * 3, pix, pix });
	idleAnimL.speed = 0.25f;

	// walk
	walkAnimR.PushBack({ pix * 0, pix * 5, pix, pix });
	walkAnimR.PushBack({ pix * 1, pix * 5, pix, pix });
	walkAnimR.PushBack({ pix * 2, pix * 5, pix, pix });
	walkAnimR.PushBack({ pix * 3, pix * 5, pix, pix });
	walkAnimR.PushBack({ pix * 4, pix * 5, pix, pix });
	walkAnimR.PushBack({ pix * 5, pix * 5, pix, pix });
	walkAnimR.PushBack({ pix * 6, pix * 5, pix, pix });
	walkAnimR.PushBack({ pix * 7, pix * 5, pix, pix });
	walkAnimR.speed = 0.25f;

	walkAnimL.PushBack({ pix * 7, pix * 5, pix, pix });
	walkAnimL.PushBack({ pix * 6, pix * 5, pix, pix });
	walkAnimL.PushBack({ pix * 5, pix * 5, pix, pix });
	walkAnimL.PushBack({ pix * 4, pix * 5, pix, pix });
	walkAnimL.PushBack({ pix * 3, pix * 5, pix, pix });
	walkAnimL.PushBack({ pix * 2, pix * 5, pix, pix });
	walkAnimL.PushBack({ pix * 1, pix * 5, pix, pix });
	walkAnimL.PushBack({ pix * 0, pix * 5, pix, pix });
	walkAnimL.speed = 0.25f;

	// death
	deathAnimR.PushBack({ pix * 0, pix * 0, pix, pix });
	deathAnimR.PushBack({ pix * 1, pix * 0, pix, pix });
	deathAnimR.PushBack({ pix * 2, pix * 0, pix, pix });
	deathAnimR.PushBack({ pix * 3, pix * 0, pix, pix });
	deathAnimR.PushBack({ pix * 4, pix * 0, pix, pix });
	deathAnimR.PushBack({ pix * 5, pix * 0, pix, pix });
	deathAnimR.PushBack({ pix * 6, pix * 0, pix, pix });
	deathAnimR.PushBack({ pix * 7, pix * 0, pix, pix });
	deathAnimR.PushBack({ pix * 8, pix * 0, pix, pix });
	deathAnimR.PushBack({ pix * 9, pix * 0, pix, pix });
	deathAnimR.loop = false;
	deathAnimR.speed = 0.05f;

	deathAnimL.PushBack({ pix * 9, pix * 0, pix, pix });
	deathAnimL.PushBack({ pix * 8, pix * 0, pix, pix });
	deathAnimL.PushBack({ pix * 7, pix * 0, pix, pix });
	deathAnimL.PushBack({ pix * 6, pix * 0, pix, pix });
	deathAnimL.PushBack({ pix * 5, pix * 0, pix, pix });
	deathAnimL.PushBack({ pix * 4, pix * 0, pix, pix });
	deathAnimL.PushBack({ pix * 3, pix * 0, pix, pix });
	deathAnimL.PushBack({ pix * 2, pix * 0, pix, pix });
	deathAnimL.PushBack({ pix * 1, pix * 0, pix, pix });
	deathAnimL.PushBack({ pix * 0, pix * 0, pix, pix });
	deathAnimL.loop = false;
	deathAnimL.speed = 0.05f;
	
}

// Destructor
Enemies::~Enemies()
{}

// Called before render is available
bool Enemies::Awake()
{

	return true;
}

// Called before the first frame
bool Enemies::Start()
{
	textureR = app->tex->Load("Assets/textures/Player.png");
	textureL = app->tex->Load("Assets/textures/PlayerInv.png");

	return true;
}

// Called each loop iteration
bool Enemies::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool Enemies::Update(float dt)
{
	for (size_t i = 0; i < enemies.Count(); i++)
	{
		enemies.At(i)->currentAnimation->Update();
		// update path
	}
	
	return true;
}

// Called each loop iteration
bool Enemies::PostUpdate()
{
	for (size_t i = 0; i < enemies.Count(); i++)
	{
		SDL_Rect rect = enemies.At(i)->currentAnimation->GetCurrentFrame();

		if (!app->scene->GetStartScreenState())
		{
			for (size_t i = 0; i < enemies.Count(); i++)
			{
				if (enemies.At(i)->lookLeft)
				{
					app->render->DrawTexture(textureL, METERS_TO_PIXELS(enemies.At(i)->x), METERS_TO_PIXELS(enemies.At(i)->y), &rect);
				}
				else
				{
					app->render->DrawTexture(textureR, METERS_TO_PIXELS(enemies.At(i)->x), METERS_TO_PIXELS(enemies.At(i)->y), &rect);
				}
			}
		}
	}
	
	return true;
}

// Called before quitting
bool Enemies::CleanUp()
{

	return true;
}

bool Enemies::LoadState(pugi::xml_node& data)
{
	for (size_t i = 0; i < data.attribute("value").as_int(); i++)
	{
		if (data.child("position" + i).attribute("state").as_int() == 0)
		{
			enemies.At(i)->x = data.child("position" + i).attribute("x").as_int();
			enemies.At(i)->y = data.child("position" + i).attribute("y").as_int();

			enemies.At(i)->body->SetTransform({ enemies.At(i)->x + PIXELS_TO_METERS(enemies.At(i)->w), enemies.At(i)->y }, enemies.At(i)->body->GetAngle());
			enemies.At(i)->body->ApplyForceToCenter({ 0, 200 }, true);

			enemies.At(i)->currentAnimation = &idleAnimR;
		}
		else
		{
			enemies.At(i)->alive = false;
		}
	}

	return true;
}

bool Enemies::SaveState(pugi::xml_node& data)
{
	for (size_t i = 0; i < enemies.Count(); i++)
	{
		data.child("position" + i).attribute("x").set_value(enemies.At(i)->x);
		data.child("position" + i).attribute("y").set_value(enemies.At(i)->y);

		if (enemies.At(i)->alive)
		{
			data.child("position" + i).attribute("state").set_value("0");
		}
		else
		{
			data.child("position" + i).attribute("state").set_value("1");
		}
	}

	data.attribute("value").set_value(enemies.Count());

	return true;
}

void Enemies::CreateGroundEnemy(float x, float y)
{
	Enemy* new_enemy = new Enemy();
	
	// body
	b2BodyDef e_body;
	e_body.type = b2_dynamicBody;
	e_body.fixedRotation = true;
	e_body.position.Set(x, y);

	new_enemy->body = app->physics->world->CreateBody(&e_body);
	new_enemy->body->SetFixedRotation(true);

	b2PolygonShape box;
	box.SetAsBox(PIXELS_TO_METERS(new_enemy->w), PIXELS_TO_METERS(new_enemy->h));

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.friction = 0.0f;
	b2Fixture* bodyFixture = new_enemy->body->CreateFixture(&fixture);
	bodyFixture->SetSensor(false);
	bodyFixture->SetUserData((void*)4); // player collision

	// ground sensor
	box.SetAsBox(PIXELS_TO_METERS((new_enemy->w - 2)), PIXELS_TO_METERS(2), b2Vec2(0, PIXELS_TO_METERS(0)), 0);
	fixture.isSensor = true;
	b2Fixture* sensorFixture = new_enemy->body->CreateFixture(&fixture);
	sensorFixture->SetUserData((void*)9); // ground sensor

	// stats
	new_enemy->origin_x = x;
	new_enemy->origin_y = y;
	new_enemy->x = x;
	new_enemy->y = y;
	new_enemy->speed = 0.3f;

	new_enemy->type = ENEMY_TYPE::GROUND;

	new_enemy->currentAnimation = &idleAnimR;

	new_enemy->lookLeft = true;

	new_enemy->detectionRange = 5.0f;
	new_enemy->enemy_spoted = false;

	new_enemy->alive = true;

	enemies.Insert(*new_enemy, enemies.Count());
}

void Enemies::CreateAirEnemy(float x, float y)
{
	Enemy* new_enemy = new Enemy();

	// body
	b2BodyDef e_body;
	e_body.type = b2_dynamicBody;
	e_body.fixedRotation = true;
	e_body.position.Set(x, y);

	new_enemy->body = app->physics->world->CreateBody(&e_body);
	new_enemy->body->SetFixedRotation(true);

	b2PolygonShape box;
	box.SetAsBox(PIXELS_TO_METERS(new_enemy->w), PIXELS_TO_METERS(new_enemy->h));

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 0.0f;
	fixture.friction = 0.0f;
	b2Fixture* bodyFixture = new_enemy->body->CreateFixture(&fixture);
	bodyFixture->SetSensor(false);
	bodyFixture->SetUserData((void*)4); // player collision

	// ground sensor
	box.SetAsBox(PIXELS_TO_METERS((new_enemy->w - 2)), PIXELS_TO_METERS(2), b2Vec2(0, PIXELS_TO_METERS(0)), 0);
	fixture.isSensor = true;
	b2Fixture* sensorFixture = new_enemy->body->CreateFixture(&fixture);
	sensorFixture->SetUserData((void*)9); // ground sensor

	// stats
	new_enemy->origin_x = x;
	new_enemy->origin_y = y;
	new_enemy->x = x;
	new_enemy->y = y;
	new_enemy->speed = 0.3f;

	new_enemy->type = ENEMY_TYPE::AIR;

	new_enemy->currentAnimation = &idleAnimR;

	new_enemy->lookLeft = true;

	new_enemy->detectionRange = 5.0f;
	new_enemy->enemy_spoted = false;

	new_enemy->alive = true;

	enemies.Insert(*new_enemy, enemies.Count());
}