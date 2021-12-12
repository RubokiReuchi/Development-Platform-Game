#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Input.h"
#include "Scene.h"
#include "Frontground.h"
#include "Menu.h"
#include "Player.h"
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
	r = { 0, 0, 2560, 1440 };

	paused = false;
	dead = false;
	lose = false;

	pause_buttons[0].state = 1;
	dead_buttons[0].state = 1;
	chosed = 0;
	app->win->GetWindowSize(win_w, win_h);

	for (size_t i = 0; i < NUM_PAUSE_BUTTONS; i++)
	{
		pause_buttons[i].rect.x = ((int)win_w / 2) - (pause_buttons[i].rect.w / 2);
		pause_buttons[i].rect.y = ((int)win_h / (NUM_PAUSE_BUTTONS + 1)) * (i + 1);
	}

	for (size_t i = 0; i < NUM_DEAD_BUTTONS; i++)
	{
		dead_buttons[i].rect.x = ((int)win_w / 2) - (dead_buttons[i].rect.w / 2);
		dead_buttons[i].rect.y = ((int)win_h / (NUM_PAUSE_BUTTONS + 3)) * (i + 2.5f);
	}

	pause_buttons[0].tex = app->tex->Load("Assets/textures/Continue.png"); // continue
	pause_buttons[1].tex = app->tex->Load("Assets/textures/Save.png"); // save
	pause_buttons[2].tex = dead_buttons[0].tex = app->tex->Load("Assets/textures/Load.png"); // load
	pause_buttons[3].tex = dead_buttons[1].tex = app->tex->Load("Assets/textures/Exit.png"); // quit
	
	gameOver = app->tex->Load("Assets/textures/Game_Over.png"); 
	cat = app->tex->Load("Assets/textures/Dead_Image.png"); 

	return true;
}

// Called each loop iteration
bool Menu::PreUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN  && !dead 
		&& app->player->currentAnimation != &app->player->deathAnimL && app->player->currentAnimation != &app->player->deathAnimR)
	{
		paused = !paused;

		pause_buttons[chosed = 0].state = 1;
		for (size_t i = 1; i < NUM_PAUSE_BUTTONS; i++)
		{
			pause_buttons[i].state = 0;
		}
	}

	if (paused && !loading && !dead)
	{
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && chosed < (NUM_PAUSE_BUTTONS - 1))
		{
			pause_buttons[chosed].state = 0;
			chosed++;
			pause_buttons[chosed].state = 1;
		}
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && chosed > 0)
		{
			pause_buttons[chosed].state = 0;
			chosed--;
			pause_buttons[chosed].state = 1;
		}
	}

	if (dead && !loading)
	{
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && chosed < (NUM_DEAD_BUTTONS - 1))
		{
			dead_buttons[chosed].state = 0;
			chosed++;
			dead_buttons[chosed].state = 1;
		}
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && chosed > 0)
		{
			dead_buttons[chosed].state = 0;
			chosed--;
			dead_buttons[chosed].state = 1;
		}
	}

	return true;
}

// Called each loop iteration
bool Menu::Update(float dt)
{
	// pause buttons
	if (paused && !loading && !dead && !lose)
	{
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
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
				app->frontground->FadeToBlack(-1);
				loading = true;
				break;
			case 3:
				return false;
				break;
			}

			pause_buttons[chosed].state = 1;
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
			paused = false;
		}
	}
	
	// dead buttons
	if (dead && !loading)
	{
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			switch (chosed)
			{
			case 0:
				app->frontground->FadeToBlack(-1);
				loading = true;
				break;
			case 1:
				return false;
				break;
			}

			dead_buttons[chosed].state = 1;
		}
	}


	//lose
	if (lose && !loading)
	{
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			switch (chosed)
			{
			case 0:
				return false;
				break;
			}

			dead_buttons[chosed].state = 1;
		}
	}

	return true;
}

// Called each loop iteration
bool Menu::PostUpdate()
{
	int c_x = -app->render->camera.x;
	r.x = c_x;

	if (paused)
	{

		app->render->DrawRectangle(r, 0, 0, 0, 200);

		for (size_t i = 0; i < NUM_PAUSE_BUTTONS; i++)
		{
			pause_buttons[i].rect.x = ((int)win_w / 2) - (pause_buttons[i].rect.w / 2) + c_x;

			if (pause_buttons[i].state == 0)
			{
				app->render->DrawRectangle(pause_buttons[i].rect, idleColorR, idleColorG, idleColorB);
			}
			else
			{
				app->render->DrawRectangle(pause_buttons[i].rect, inColorR, inColorG, inColorB);
			}

			app->render->DrawTexture(pause_buttons[i].tex, pause_buttons[i].rect.x + 10, pause_buttons[i].rect.y + 10);
		}
	}

	if (dead)
	{
		app->render->DrawRectangle(r, 0, 0, 0, 200);

		app->render->DrawTexture(gameOver, 0 + c_x, 75);
		app->render->DrawTexture(cat, 950 + c_x, 800);

		for (size_t i = 0; i < NUM_DEAD_BUTTONS; i++)
		{
			dead_buttons[i].rect.x = ((int)win_w / 2) - (dead_buttons[i].rect.w / 2) + c_x;

			if (dead_buttons[i].state == 0)
			{
				app->render->DrawRectangle(dead_buttons[i].rect, idleColorR, idleColorG, idleColorB);
			}
			else
			{
				app->render->DrawRectangle(dead_buttons[i].rect, inColorR, inColorG, inColorB);
			}

			app->render->DrawTexture(dead_buttons[i].tex, dead_buttons[i].rect.x + 10, dead_buttons[i].rect.y + 10);
		}
	}


	if (lose)
	{
		app->render->DrawRectangle(r, 0, 0, 0, 200);

		app->render->DrawTexture(gameOver, 0 + c_x, 75);
		app->render->DrawTexture(cat, 950 + c_x, 800);

		dead_buttons[1].rect.x = ((int)win_w / 2) - (dead_buttons[1].rect.w / 2) + c_x;
		
		app->render->DrawRectangle(dead_buttons[1].rect, inColorR, inColorG, inColorB);

		app->render->DrawTexture(dead_buttons[1].tex, dead_buttons[1].rect.x + 10, dead_buttons[1].rect.y + 10);
		
	}

	if (!paused && !dead && !lose && !app->scene->GetStartScreenState())
	{
		SDL_Rect hab_rect = app->player->habAnimation->GetCurrentFrame();
		uint win_w, win_h;
		app->win->GetWindowSize(win_w, win_h);
		int hab_h = 2 * win_h;

		if (app->player->hab_ready)
		{
			app->render->DrawRectangle({ c_x + 20, hab_h - 129, 109, 109 }, 0, 0, 255);
		}
		else
		{
			app->render->DrawRectangle({ c_x + 20, hab_h - 129, 109, 109 }, 255, 0, 0);
		}
		
		app->render->DrawTexture(app->player->hab_tex, c_x + 20, hab_h - 129, &hab_rect);
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