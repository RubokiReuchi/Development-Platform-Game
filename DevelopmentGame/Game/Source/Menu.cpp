#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Input.h"
#include "Scene.h"
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

	buttons[0].tex = app->tex->Load("Assets/textures/Continue_text.png");
	buttons[1].tex = app->tex->Load("Assets/textures/Continue_text.png");
	buttons[2].tex = app->tex->Load("Assets/textures/Continue_text.png");
	buttons[3].tex = app->tex->Load("Assets/textures/Continue_text.png");

	return true;
}

// Called each loop iteration
bool Menu::PreUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		// freeze game
		paused = !paused;
	}

	if (paused)
	{
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		{
			buttons[chosed].state = 0;
			chosed++;
			buttons[chosed].state = 1;
		}
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
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
	if (paused)
	{
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			buttons[chosed].state = 2;

			switch (chosed)
			{
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			default:
				break;
			}
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
			else if (buttons[i].state == 1)
			{
				app->render->DrawRectangle(buttons[i].rect, inColorR, inColorG, inColorB);
			}
			else
			{
				app->render->DrawRectangle(buttons[i].rect, clickedColorR, clickedColorG, clickedColorB);
			}

			app->render->DrawTexture(buttons[i].tex, buttons[i].rect.x, buttons[i].rect.y);
		}
	}
	

	return true;
}

// Called before quitting
bool Menu::CleanUp()
{

	return true;
}