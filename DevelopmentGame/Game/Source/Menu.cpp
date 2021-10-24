#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Input.h"
#include "Scene.h"
#include "Frontground.h"
#include "Menu.h"

#include "Defs.h"
#include "Log.h"

Menu::Menu() : Module()
{
	name.Create("menu");
}

// Destructor
Menu::~Menu()
{}

// Called before render is available
bool Menu::Awake()
{

	return true;
}

// Called before the first frame
bool Menu::Start()
{
	r = { 0, 0, 1280, 1280 };
	paused = false;

	buttons[0].state = 1;
	chosed = 0;
	uint win_w, win_h;
	app->win->GetWindowSize(win_w, win_h);

	for (size_t i = 0; i < NUM_BUTTONS; i++)
	{
		buttons[i].rect.x = ((int)win_w / 2) - (buttons[0].rect.w / 2);
		buttons[i].rect.y = ((int)win_h / (NUM_BUTTONS + 1)) * (i + 1);
	}

	buttons[0].tex = app->tex->Load("Assets/textures/Continue_text.png"); // continue
	buttons[1].tex = app->tex->Load("Assets/textures/Continue_text.png"); // save
	buttons[2].tex = app->tex->Load("Assets/textures/Continue_text.png"); // load
	buttons[3].tex = app->tex->Load("Assets/textures/Continue_text.png"); // quit

	return true;
}

// Called each loop iteration
bool Menu::PreUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		paused = !paused;

		buttons[chosed = 0].state = 1;
		for (size_t i = 1; i < NUM_BUTTONS; i++)
		{
			buttons[i].state = 0;
		}
	}

	if (paused && !loading)
	{
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && chosed < (NUM_BUTTONS - 1))
		{
			buttons[chosed].state = 0;
			chosed++;
			buttons[chosed].state = 1;
		}
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && chosed > 0)
		{
			buttons[chosed].state = 0;
			chosed--;
			buttons[chosed].state = 1;
		}
	}

	return true;
}

// Called each loop iteration
bool Menu::Update(float dt)
{
	if (paused && !loading)
	{
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			switch (chosed)
			{
			case 0: 
				paused = false;
				break;
			case 1:
				app->SaveGameRequest();
				break;
			case 2:
				app->frontground->FadeToBlack();
				loading = true;
				break;
			case 3:
				return false;
				break;
			}

			buttons[chosed].state = 1;
		}
	}

	// fade at load
	if (loading)
	{
		load_cd--;
		if (load_cd <= 0)
		{
			app->LoadGameRequest();
			load_cd = 120;
			loading = false;
		}
	}
	

	return true;
}

// Called each loop iteration
bool Menu::PostUpdate()
{
	if (paused)
	{
		app->render->DrawRectangle(r, 0, 0, 0, 200);

		for (size_t i = 0; i < NUM_BUTTONS; i++)
		{
			if (buttons[i].state == 0)
			{
				app->render->DrawRectangle(buttons[i].rect, idleColorR, idleColorG, idleColorB);
			}
			else
			{
				app->render->DrawRectangle(buttons[i].rect, inColorR, inColorG, inColorB);
			}

			app->render->DrawTexture(buttons[i].tex, buttons[i].rect.x + 10, buttons[i].rect.y + 10);
		}
	}
	

	return true;
}

// Called before quitting
bool Menu::CleanUp()
{

	return true;
}

bool Menu::GetGameState()
{
	return paused;
}