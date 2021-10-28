#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "Frontground.h"

#include "Defs.h"
#include "Log.h"

Frontground::Frontground() : Module()
{
	name.Create("frontground");
	a = 0;
}

// Destructor
Frontground::~Frontground()
{}

// Called before render is available
bool Frontground::Awake()
{

	return true;
}

// Called before the first frame
bool Frontground::Start()
{
	r = { 0, 0, 2560, 1440 };

	press_e = app->tex->Load("Assets/textures/Save.png");

	return true;
}

// Called each loop iteration
bool Frontground::PreUpdate()
{
	if (go_black)
	{
		if (a < 256 - fade_speed)
		{
			a += fade_speed;
		}
		else if (a < 255)
		{
			a++;
		}
	}
	else if (return_black)
	{
		if (a > fade_speed)
		{
			a -= fade_speed;
		}
		else if (a > 0)
		{
			a--;
		}
	}

	return true;
}

// Called each loop iteration
bool Frontground::Update(float dt)
{
	if (a >= 255)
	{
		go_black = false;
		FadeFromBlack(destination_level);
		
	}
	else if (a <= 0)
	{
		return_black = false;
	}

	return true;
}

// Called each loop iteration
bool Frontground::PostUpdate()
{
	int c_x = -app->render->camera.x;
	r.x = c_x;

	if (!press_e_hide)
	{
		app->render->DrawTexture(press_e, c_x + 1280, 1000);
	}

	app->render->DrawRectangle(r, 0, 0, 0, a);

	return true;
}

// Called before quitting
bool Frontground::CleanUp()
{

	return true;
}

bool Frontground::FadeToBlack(int dest_level)
{
	go_black = true;
	if (dest_level != -1) destination_level = dest_level;

	return true;
}

bool Frontground::FadeFromBlack(int dest_level)
{
	return_black = true;

	if (dest_level != -1)
	{
		app->map->CleanMaps();
		app->physics->CleanMapBoxes();
		app->map->collision_loaded = false;

		switch (dest_level)
		{
		case 0:
			app->scene->ReturnStartScreen();
			break;
		case 1: 
			app->player->SetPosition(8.2f, 22.5f);
			app->SaveGameRequest();
			app->player->lookLeft = false;
			app->map->Load("level1.tmx");
			app->scene->current_level = 1;
			break;
		case 2:
			app->player->SetPosition(48.0f, 4.0f);
			app->SaveGameRequest();
			app->player->lookLeft = true;
			app->map->Load("level2.tmx");
			app->scene->current_level = 2;
			break;
		}
	}

	return true;
}