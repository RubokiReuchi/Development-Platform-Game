#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Frontground.h"

#include "Defs.h"
#include "Log.h"

Frontground::Frontground() : Module()
{
	name.Create("frontground");
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
	r = { 0, 0, 1280, 1280 };

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
		else
		{
			a++;
		}
	}
	else if (return_black)
	{
		if (a >  fade_speed)
		{
			a -= fade_speed;
		}
		else
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
		FadeFromBlack();
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
	app->render->DrawRectangle(r, 0, 0, 0, a);

	return true;
}

// Called before quitting
bool Frontground::CleanUp()
{

	return true;
}

bool Frontground::FadeToBlack()
{
	go_black = true;

	return true;
}

bool Frontground::FadeFromBlack()
{
	return_black = true;

	return true;
}