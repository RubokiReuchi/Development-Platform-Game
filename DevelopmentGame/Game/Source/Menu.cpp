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
	
	//SDL_PumpEvents();  // make sure we have the latest mouse state.

	//buttons = SDL_GetMouseState(&x, &y);


	r = { 0, 0, 2560, 1440 };
	paused = false;
	settings = false;
	dead = false;
	lose = false;

	pause_buttons[0].state = 1;
	dead_buttons[0].state = 1;
	settings_buttons[0].state = 1;
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

	for (size_t i = 0; i < NUM_SETTINGS_BUTTONS; i++)
	{
		settings_buttons[i].rect.x = ((int)win_w / 2) - (pause_buttons[i].rect.w / 2);
		settings_buttons[i].rect.y = ((int)win_h / (NUM_PAUSE_BUTTONS + 1)) * (i + 1);
	}

	pause_buttons[0].tex = app->tex->Load("Assets/textures/Continue.png"); // Continue
	pause_buttons[1].tex = app->tex->Load("Assets/textures/Settings.png"); // Settings
	pause_buttons[2].tex = app->tex->Load("Assets/textures/Back.png"); // Back to Menu
	pause_buttons[3].tex = dead_buttons[1].tex = app->tex->Load("Assets/textures/Exit.png"); // Exit
	dead_buttons[0].tex = app->tex->Load("Assets/textures/Load.png"); // load

	settings_buttons[0].tex = app->tex->Load("Assets/textures/Continue.png"); // Audio slider
	settings_buttons[1].tex = app->tex->Load("Assets/textures/Settings.png"); // Fx slider
	settings_buttons[2].tex = app->tex->Load("Assets/textures/Fullscreen_no.png"); // Fullscreen
	settings_buttons[2].texfullscreen = app->tex->Load("Assets/textures/Fullscreen_si.png"); // Fullscreen
	settings_buttons[3].tex = app->tex->Load("Assets/textures/Vsync_no.png"); // Vsync
	settings_buttons[3].texvsync = app->tex->Load("Assets/textures/Vsync_si.png"); // Vsync

	gameOver = app->tex->Load("Assets/textures/Game_Over.png"); 
	cat = app->tex->Load("Assets/textures/Dead_Image.png"); 

	return true;
}

// Called each loop iteration
bool Menu::PreUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN  && !dead )
	{
		paused = !paused;
		settings = false;
		vsync = false;
		fullscreen = false;

		pause_buttons[chosed = 0].state = 1;
		for (size_t i = 1; i < NUM_PAUSE_BUTTONS; i++)
		{
			pause_buttons[i].state = 0;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && paused &&!dead && chosed == 1)
	{
		settings = !settings;

		settings_buttons[chosed = 0].state = 1;
		for (size_t i = 1; i < NUM_SETTINGS_BUTTONS; i++)
		{
			settings_buttons[i].state = 0;
		}
	}

	if (paused && !settings && !loading && !dead)
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


	if (settings && !loading && !dead)
	{
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && chosed < (NUM_SETTINGS_BUTTONS - 1))
		{
			settings_buttons[chosed].state = 0;
			chosed++;
			settings_buttons[chosed].state = 1;
		}
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && chosed > 0)
		{
			settings_buttons[chosed].state = 0;
			chosed--;
			settings_buttons[chosed].state = 1;
		}
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && chosed == 3)
		{
			settings_buttons[chosed].state = 2;
			vsync = !vsync;

		}
		if (chosed == 2 && app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			settings_buttons[chosed].state = 2;
			fullscreen = !fullscreen;

		}
		
	}

	/*
	for (size_t i = 1; i < NUM_PAUSE_BUTTONS; i++)
	{
		if (x > pause_buttons[i].rect.x)
		{
			pause_buttons[i].state = 1;
		}
	}*/

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
				settings = false;
				break;
			case 3:
				return false;
				break;
			}
			
			pause_buttons[chosed].state = 2;
		}
	}

	if (paused && settings && !loading && !dead && !lose)
	{
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			/*switch (chosed)
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
			}*/

			pause_buttons[chosed].state = 2;
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
			dead_buttons[chosed].state = 2;
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

			dead_buttons[chosed].state = 2;
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
			pause_buttons[i].rect.x = ((int)win_w / 2) - (pause_buttons[i].rect.w / 2) + c_x -300;

			if (pause_buttons[i].state == 0)
			{
				app->render->DrawRectangle(pause_buttons[i].rect, idleColorR, idleColorG, idleColorB);
			}
			else if(pause_buttons[i].state == 1)
			{
				app->render->DrawRectangle(pause_buttons[i].rect, inColorR, inColorG, inColorB);
			}
			else if (pause_buttons[i].state == 2)
			{
				app->render->DrawRectangle(pause_buttons[i].rect, pColorR, pColorG, pColorB);
			}

			app->render->DrawTexture(pause_buttons[i].tex, pause_buttons[i].rect.x + 10, pause_buttons[i].rect.y + 10);
		}
	}

	if (settings)
	{
		app->render->DrawRectangle(r, 0, 0, 0, 200);

		for (size_t i = 0; i < NUM_SETTINGS_BUTTONS; i++)
		{
			settings_buttons[i].rect.x = ((int)win_w / 2) - (settings_buttons[i].rect.w / 2) + c_x;

			if (settings_buttons[i].state == 0)
			{
				app->render->DrawRectangle(settings_buttons[i].rect, idleColorR, idleColorG, idleColorB);
			}
			else if (settings_buttons[i].state == 1)
			{
				app->render->DrawRectangle(settings_buttons[i].rect, inColorR, inColorG, inColorB);
			}
			else if (settings_buttons[i].state == 2)
			{
				app->render->DrawRectangle(settings_buttons[i].rect, pColorR, pColorG, pColorB);
			}
			if (vsync)
			{
				app->render->DrawTexture(settings_buttons[i].texvsync, settings_buttons[i].rect.x + 10, settings_buttons[i].rect.y + 10);
			}
			if (fullscreen)
			{
				app->render->DrawTexture(settings_buttons[i].texfullscreen, settings_buttons[i].rect.x + 10, settings_buttons[i].rect.y + 10);
			}

			app->render->DrawTexture(settings_buttons[i].tex, settings_buttons[i].rect.x + 10, settings_buttons[i].rect.y + 10);
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
			else if(dead_buttons[i].state == 1)
			{
				app->render->DrawRectangle(dead_buttons[i].rect, inColorR, inColorG, inColorB);
			}
			else if (dead_buttons[i].state == 2)
			{
				app->render->DrawRectangle(dead_buttons[i].rect, pColorR, pColorG, pColorB);

				app->render->DrawTexture(dead_buttons[i].tex, dead_buttons[i].rect.x + 10, dead_buttons[i].rect.y + 10);
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