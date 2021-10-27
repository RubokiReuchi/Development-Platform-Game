#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Frontground.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	start_screen = app->tex->Load("Assets/textures/Start_screen.png");
	
	// Load music
	app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{

	if (start_screen != NULL && app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)  
	{
		PassLevel(0);
	}

	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		PassLevel(0);
	}
	else if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		PassLevel(1);
	}
	else if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		PassLevel(current_level - 1);
	}
	else if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		app->SaveGameRequest();
	}
	else if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		app->LoadGameRequest();
	}
	else if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{

	}


	// Draw map
	if (start_screen != NULL)
	{
		app->map->Draw();
	}

	// L03: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
				   app->map->mapData.width, app->map->mapData.height,
				   app->map->mapData.tileWidth, app->map->mapData.tileHeight,
				   app->map->mapData.tilesets.count());

	app->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (start_screen != NULL) 
	{
		app->render->DrawTexture(start_screen, 0, 0);
	}
	else
	{
		app->map->Draw();
	}
	
	if (app->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool Scene::GetStartScreenState()
{
	if (start_screen != NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Scene::PassLevel(int c_level)
{
	switch (c_level)
	{
	case 0:
		app->frontground->FadeFromBlack();
		if (start_screen != NULL)
		{
			app->frontground->FadeFromBlack();
		}
		else
		{
			app->frontground->FadeToBlack();
		}
		app->player->SetPosition(8.2f, 22.5f);
		app->map->Load("level1.tmx");
		current_level = 1;
		break;
	}

	start_screen = NULL;

	return true;
}