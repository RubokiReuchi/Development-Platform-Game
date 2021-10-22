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
	fixture.friction = 0.0f;
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
	//debug
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		player_body->SetTransform({ x, 0 }, player_body->GetAngle());
	}

	// move left
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		player_body->SetLinearVelocity({ -speed, player_body->GetLinearVelocity().y });
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		player_body->SetLinearVelocity({ 0, player_body->GetLinearVelocity().y });
	}

	//move right
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		player_body->SetLinearVelocity({ speed, player_body->GetLinearVelocity().y });
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		player_body->SetLinearVelocity({ 0, player_body->GetLinearVelocity().y });
	}

	//jump
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		player_body->SetLinearVelocity({ player_body->GetLinearVelocity().x , 0 });
		player_body->ApplyForceToCenter({ 0, -jumpForce }, true);
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