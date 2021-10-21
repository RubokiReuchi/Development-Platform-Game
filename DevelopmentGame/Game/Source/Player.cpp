#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Physics.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

Player::Player() : Module()
{
	name.Create("player");
}

// Destructor
Player::~Player()
{}

// Called before render is available
bool Player::Awake()
{

	return true;
}

// Called before the first frame
bool Player::Start()
{
	b2BodyDef p_body;
	p_body.type = b2_dynamicBody;
	p_body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	
	player_body = app->physics->world->CreateBody(&p_body);
	player_body->SetBullet(true);
	player_body->SetFixedRotation(true);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(w), PIXEL_TO_METERS(h));

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	player_body->CreateFixture(&fixture);

	return true;
}

// Called each loop iteration
bool Player::PreUpdate()
{
	x = player_body->GetPosition().x;
	y = player_body->GetPosition().y;

	return true;
}

// Called each loop iteration
bool Player::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		player_body->SetTransform({ x, 0 }, player_body->GetAngle());
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		player_body->SetTransform({ x -= speed, y }, player_body->GetAngle());
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		player_body->SetTransform({ x += speed, y }, player_body->GetAngle());
	}
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{

		player_body->ApplyForce({ 0, -jumpForce }, player_body->GetWorldCenter(), true);
	}

	return true;
}

// Called each loop iteration
bool Player::PostUpdate()
{

	return true;
}

// Called before quitting
bool Player::CleanUp()
{

	return true;
}