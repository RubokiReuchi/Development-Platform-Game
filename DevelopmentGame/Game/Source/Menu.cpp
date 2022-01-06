#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Audio.h"
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
	settings = false;
	dead = false;
	lose = false;
	slider = false;
	slider2 = false;
	fullscreen = false;
	vsync = false;
	credits = false;

	pause_buttons[0].state = 1;
	menu_buttons[0].state = 1;
	dead_buttons[0].state = 1;
	settings_buttons[0].state = 1;
	chosed = 0;
	app->win->GetWindowSize(win_w, win_h);

	

	for (size_t i = 0; i < NUM_PAUSE_BUTTONS; i++)
	{
		pause_buttons[i].rect.x = ((int)win_w / 2) - (pause_buttons[i].rect.w / 2);
		pause_buttons[i].rect.y = ((int)win_h / (NUM_PAUSE_BUTTONS + 1)) * (i + 1);
	}

	for (size_t i = 0; i < NUM_MENU_BUTTONS; i++)
	{
		menu_buttons[i].rect.x = ((int)win_w / 2) - (menu_buttons[i].rect.w / 2);
		menu_buttons[i].rect.y = ((int)win_h / (NUM_MENU_BUTTONS + 1)) * (i + 1);
	}

	for (size_t i = 0; i < NUM_DEAD_BUTTONS; i++)
	{
		dead_buttons[i].rect.x = ((int)win_w / 2) - (dead_buttons[i].rect.w / 2);
		dead_buttons[i].rect.y = ((int)win_h / (NUM_PAUSE_BUTTONS + 3)) * (i + 2.5f);
	}

	for (size_t i = 0; i < NUM_SETTINGS_BUTTONS; i++)
	{
		settings_buttons[i].rect.x = ((int)win_w / 2) - (settings_buttons[i].rect.w / 2);
		settings_buttons[i].rect.y = ((int)win_h / (NUM_PAUSE_BUTTONS + 1)) * (i + 1);
	}

	pause_buttons[0].tex = app->tex->Load("Assets/textures/Continue.png"); // Continue
	pause_buttons[1].tex = app->tex->Load("Assets/textures/Settings.png"); // Settings
	pause_buttons[2].tex = app->tex->Load("Assets/textures/Back.png"); // Back to Menu
	pause_buttons[3].tex = app->tex->Load("Assets/textures/Exit.png"); // Exit
	

	dead_buttons[0].tex = app->tex->Load("Assets/textures/Load.png"); // Load
	dead_buttons[1].tex = app->tex->Load("Assets/textures/Exit.png"); //Exit

	menu_buttons[0].tex = app->tex->Load("Assets/textures/Play.png"); // Play
	menu_buttons[1].tex = app->tex->Load("Assets/textures/Continue.png"); // Continue
	menu_buttons[2].tex = app->tex->Load("Assets/textures/Settings.png"); // Settings
	menu_buttons[3].tex = app->tex->Load("Assets/textures/Credits.png"); // Credits
	menu_buttons[3].texcredits = app->tex->Load("Assets/textures/CreditsImage.png"); // Credits
	menu_buttons[4].tex = app->tex->Load("Assets/textures/Exit.png"); // Exit

	settings_buttons[0].texslider = settings_buttons[1].texslider = app->tex->Load("Assets/textures/Slider.png"); // Slider
	settings_buttons[0].tex = app->tex->Load("Assets/textures/Sound.png"); // Audio
	settings_buttons[1].tex = app->tex->Load("Assets/textures/Fx.png"); // Fx slider
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
	intro = app->scene->GetStartScreenState();
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN  && !dead && intro == false)
	{
		paused = !paused;
	}

	if (intro)
	{
		paused = true;
	}

	if (settings && app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		settings = false;
	}

	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_PRESSED && paused && !intro && !dead && chosed == 1 && pause_buttons[1].state == 1)
	{
		settings = !settings;
		paused = !paused;
	}

	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_PRESSED && paused && intro && !dead && chosed == 2 && menu_buttons[2].state == 1)
	{
		settings = true;
		paused = !paused;
	}



	//MOVIMIENTO MANUAL
	/*
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
	}
	*/
	
	int x, y;
	app->input->GetMousePosition(x, y);
	float c = -app->render->camera.x;
	for (size_t i = 0; i < NUM_PAUSE_BUTTONS; i++)
	{
		SDL_Rect rect = pause_buttons[i].rect;
		if (x + c > rect.x && x + c < rect.x + rect.w && y > rect.y && y < rect.y + rect.h)
		{
			chosed = i;
			pause_buttons[i].state = 1;
		}
		else 
		{
			pause_buttons[i].state = 0;
		}
	}

	for (size_t i = 0; i < NUM_MENU_BUTTONS; i++)
	{
		SDL_Rect rect = menu_buttons[i].rect;
		if (x + c > rect.x && x + c < rect.x + rect.w && y > rect.y && y < rect.y + rect.h)
		{
			chosed = i;
			menu_buttons[i].state = 1;
		}
		else
		{
			menu_buttons[i].state = 0;
		}
	}

	for (size_t i = 0; i < NUM_SETTINGS_BUTTONS; i++)
	{
		SDL_Rect rect = settings_buttons[i].rect;
		if (x + c > 530 && x + c < 530 + rect.w && y > rect.y && y < rect.y + rect.h)
		{
			chosed = i;
			settings_buttons[i].state = 1;
		}
		else
		{
			settings_buttons[i].state = 0;
		}
	}

	for (size_t i = 0; i < NUM_DEAD_BUTTONS; i++)
	{
		SDL_Rect rect = dead_buttons[i].rect;
		if (x + c > rect.x && x + c < rect.x + rect.w && y > rect.y && y < rect.y + rect.h)
		{
			chosed = i;
			dead_buttons[i].state = 1;
		}
		else
		{
			dead_buttons[i].state = 0;
		}
	}

	return true;
}

// Called each loop iteration
bool Menu::Update(float dt)
{
	// pause buttons
	if (paused && !intro && !settings)
	{
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_PRESSED && pause_buttons[chosed].state == 1)
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
				//aun aparecen las monedas etc
				app->scene->ReturnStartScreen();
				intro = false;
				paused = false;
				break;
			case 3:
				return false;
				break;
			}
			
			pause_buttons[chosed].state = 0;
		}
	}

	//menu buttons
	if (intro && !settings)
	{
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_PRESSED && menu_buttons[chosed].state == 1)
		{
			switch (chosed)
			{
			case 0:
				app->scene->PassLevel(1);
				intro = false;
				paused = false;
				break;
			case 1:
				app->frontground->FadeToBlack(-1);
				loading = true;
				break;
			case 2:
				settings = true;
			case 3:
				credits = !credits;
				break;
			case 4:
				return false;
				break;
			}

			menu_buttons[chosed].state = 0;
		}
	}

	//settings buttons
	if (settings)
	{
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_PRESSED && settings_buttons[chosed].state == 1)
		{
			switch (chosed)
			{
			case 0:
				slider = !slider;
				break;
			case 1:
				slider2 = !slider2;
				break;
			case 2:
				fullscreen = !fullscreen;
				if (fullscreen)
				{
					SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
				}
				else
				{
					SDL_SetWindowFullscreen(app->win->window, 0);
				}
				break;
			case 3:
				vsync = !vsync;
				if (vsync)
				{
					// 1 to activate, 0 to deactivate
					SDL_GL_SetSwapInterval(1);
				}
				else
				{
					SDL_GL_SetSwapInterval(0);
				}
				break;
			}

			settings_buttons[chosed].state = 0;
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
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_PRESSED && dead_buttons[chosed].state == 1)
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

			dead_buttons[chosed].state = 0;
		}
	}


	//lose
	if (lose && !loading)
	{
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			return false;
		}
	}

	return true;
}

// Called each loop iteration
bool Menu::PostUpdate()
{
	int c_x = -app->render->camera.x;
	r.x = c_x;

	if (app->scene->GetStartScreenState() != NULL)
	{
		app->render->DrawTexture(app->tex->Load("Assets/textures/Start_screen.png"), 0, 0);
	}

	if (paused && !intro && !settings)
	{
		if (intro == false)
		{
			app->render->DrawRectangle(r, 0, 0, 0, 200);
		}

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

	if (intro && !settings)
	{
		for (size_t i = 0; i < NUM_MENU_BUTTONS; i++)
		{
			menu_buttons[i].rect.x = ((int)win_w / 2) - (menu_buttons[i].rect.w / 2) + c_x - 300;

			if (menu_buttons[i].state == 0)
			{
				app->render->DrawRectangle(menu_buttons[i].rect, idleColorR, idleColorG, idleColorB);
			}
			else if (menu_buttons[i].state == 1)
			{
				app->render->DrawRectangle(menu_buttons[i].rect, inColorR, inColorG, inColorB);
			}
			else if (menu_buttons[i].state == 2)
			{
				app->render->DrawRectangle(menu_buttons[i].rect, pColorR, pColorG, pColorB);
			}

			if (credits)
			{
				app->render->DrawTexture(menu_buttons[i].texcredits, 1000, 500);
			}

			app->render->DrawTexture(menu_buttons[i].tex, menu_buttons[i].rect.x + 10, menu_buttons[i].rect.y + 10);
		}
	}

	if (settings)
	{
		
		int z, w;
		app->input->GetMousePosition(z, w);

		if (intro == false)
		{
			app->render->DrawRectangle(r, 0, 0, 0, 200);
		}

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

			if (slider)
			{
				if (z < 540)
				{
					z = 540;
				}
				else if (z > 729)
				{
					z = 729;
				}
				xbarra = z;
				app->render->DrawTexture(settings_buttons[0].texslider, z, settings_buttons[0].rect.y + 10);
				app->audio->SetMusic((z - 540) / 2);
			}
			else
			{
				app->render->DrawTexture(settings_buttons[0].texslider, xbarra, settings_buttons[0].rect.y + 10);
			}

			if (slider2)
			{
				if (z < 540)
				{
					z = 540;
				}
				else if (z > 729)
				{
					z = 729;
				}
				xbarra2 = z;
				app->render->DrawTexture(settings_buttons[1].texslider, z, settings_buttons[1].rect.y + 10);
				app->audio->SetFX((z - 540) / 2);
			}
			else
			{
				app->render->DrawTexture(settings_buttons[1].texslider, xbarra2, settings_buttons[1].rect.y + 10);
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