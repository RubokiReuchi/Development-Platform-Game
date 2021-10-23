#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Background.h"

#include "Defs.h"
#include "Log.h"

Background::Background() : Module()
{
	name.Create("background");
}

// Destructor
Background::~Background()
{}

// Called before render is available
bool Background::Awake()
{

	return true;
}

// Called before the first frame
bool Background::Start()
{
	r = { 0, 0, 1280, 1280 };
	texture = app->tex->Load("Assets/textures/background.png");

	return true;
}

// Called each loop iteration
bool Background::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool Background::Update(float dt)
{

	return true;
}

// Called each loop iteration
bool Background::PostUpdate()
{
	app->render->DrawTexture(texture, 0, 0, &r);

	return true;
}

// Called before quitting
bool Background::CleanUp()
{

	return true;
}
