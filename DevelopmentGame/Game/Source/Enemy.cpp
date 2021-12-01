#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Physics.h"
#include "Player.h"
#include "Menu.h"

#include "Defs.h"
#include "Log.h"

Enemies::Enemies() : Module()
{
	name.Create("enemies");
	
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
	currentAnimation->Update();
	
	return true;
}

// Called each loop iteration
bool Enemies::PostUpdate()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	 
	if (!app->scene->GetStartScreenState())
	{
		for (size_t i = 0; i < enemies.Count(); i++)
		{
			if (lookLeft)
			{
				app->render->DrawTexture(textureL, METERS_TO_PIXELS(enemies.At(i)->x), METERS_TO_PIXELS(enemies.At(i)->y), &rect);
			}
			else
			{
				app->render->DrawTexture(textureR, METERS_TO_PIXELS(enemies.At(i)->x), METERS_TO_PIXELS(enemies.At(i)->y), &rect);
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
	x = data.child("position").attribute("x").as_int();
	y = data.child("position").attribute("y").as_int();

	player_body->SetTransform({ x + PIXELS_TO_METERS(w), y }, player_body->GetAngle());
	player_body->ApplyForceToCenter({ 0, 200 }, true);

	currentAnimation = &idleAnimR;
	if (app->menu->dead)
	{
		app->menu->dead = false;
	}

	return true;
}

bool Enemies::SaveState(pugi::xml_node& data)
{
	data.child("position").attribute("x").set_value(x);
	data.child("position").attribute("y").set_value(y);

	return true;
}

void CreateGroundEnemy(float x, float y)
{
	Enemy* new_enemy = new Enemy();
	
	// body

	// stats
	new_enemy->x = x;
	new_enemy->y = y;
	new_enemy->speed = 0.3f;
}

void CreateAirEnemy(float x, float y)
{

}