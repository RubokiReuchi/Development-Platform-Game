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
#include "Menu.h"

#include "Defs.h"
#include "Log.h"

Player::Player() : Module()
{
	name.Create("player");
	int pix = 111;

	// idle animation
	idleAnimR.PushBack({ pix * 0, pix * 3, pix, pix });
	idleAnimR.PushBack({ pix * 1, pix * 3, pix, pix });
	idleAnimR.PushBack({ pix * 2, pix * 3, pix, pix });
	idleAnimR.PushBack({ pix * 3, pix * 3, pix, pix });
	idleAnimR.PushBack({ pix * 4, pix * 3, pix, pix });
	idleAnimR.PushBack({ pix * 5, pix * 3, pix, pix });
	idleAnimR.PushBack({ pix * 6, pix * 3, pix, pix });
	idleAnimR.PushBack({ pix * 7, pix * 3, pix, pix });
	idleAnimR.PushBack({ pix * 8, pix * 3, pix, pix });
	idleAnimR.PushBack({ pix * 9, pix * 3, pix, pix });
	idleAnimR.speed = 0.25f;

	idleAnimL.PushBack({ pix * 9, pix * 3, pix, pix });
	idleAnimL.PushBack({ pix * 8, pix * 3, pix, pix });
	idleAnimL.PushBack({ pix * 7, pix * 3, pix, pix });
	idleAnimL.PushBack({ pix * 6, pix * 3, pix, pix });
	idleAnimL.PushBack({ pix * 5, pix * 3, pix, pix });
	idleAnimL.PushBack({ pix * 4, pix * 3, pix, pix });
	idleAnimL.PushBack({ pix * 3, pix * 3, pix, pix });
	idleAnimL.PushBack({ pix * 2, pix * 3, pix, pix });
	idleAnimL.PushBack({ pix * 1, pix * 3, pix, pix });
	idleAnimL.PushBack({ pix * 0, pix * 3, pix, pix });
	idleAnimL.speed = 0.25f;

	// jump
	jumpAnimR.PushBack({ pix * 0, pix * 4, pix, pix });
	jumpAnimR.PushBack({ pix * 1, pix * 4, pix, pix });
	jumpAnimR.PushBack({ pix * 2, pix * 4, pix, pix });
	jumpAnimR.PushBack({ pix * 3, pix * 4, pix, pix });
	jumpAnimR.PushBack({ pix * 4, pix * 4, pix, pix });
	jumpAnimR.PushBack({ pix * 5, pix * 4, pix, pix });
	jumpAnimR.PushBack({ pix * 6, pix * 4, pix, pix });
	jumpAnimR.PushBack({ pix * 7, pix * 4, pix, pix });
	jumpAnimR.loop = false;
	jumpAnimR.speed = 0.15f;

	jumpAnimL.PushBack({ pix * 7, pix * 4, pix, pix });
	jumpAnimL.PushBack({ pix * 6, pix * 4, pix, pix });
	jumpAnimL.PushBack({ pix * 5, pix * 4, pix, pix });
	jumpAnimL.PushBack({ pix * 4, pix * 4, pix, pix });
	jumpAnimL.PushBack({ pix * 3, pix * 4, pix, pix });
	jumpAnimL.PushBack({ pix * 2, pix * 4, pix, pix });
	jumpAnimL.PushBack({ pix * 1, pix * 4, pix, pix });
	jumpAnimL.PushBack({ pix * 0, pix * 4, pix, pix });
	jumpAnimL.loop = false;
	jumpAnimL.speed = 0.15f;

	// fall
	fallAnimR.PushBack({ pix * 0, pix * 1, pix, pix });
	fallAnimR.PushBack({ pix * 1, pix * 1, pix, pix });
	fallAnimR.PushBack({ pix * 2, pix * 1, pix, pix });
	fallAnimR.PushBack({ pix * 3, pix * 1, pix, pix });
	fallAnimR.PushBack({ pix * 4, pix * 1, pix, pix });
	fallAnimR.PushBack({ pix * 5, pix * 1, pix, pix });
	fallAnimR.PushBack({ pix * 6, pix * 1, pix, pix });
	fallAnimR.PushBack({ pix * 7, pix * 1, pix, pix });
	fallAnimR.loop = false;
	fallAnimR.speed = 0.15f;

	fallAnimL.PushBack({ pix * 7, pix * 1, pix, pix });
	fallAnimL.PushBack({ pix * 6, pix * 1, pix, pix });
	fallAnimL.PushBack({ pix * 5, pix * 1, pix, pix });
	fallAnimL.PushBack({ pix * 4, pix * 1, pix, pix });
	fallAnimL.PushBack({ pix * 3, pix * 1, pix, pix });
	fallAnimL.PushBack({ pix * 2, pix * 1, pix, pix });
	fallAnimL.PushBack({ pix * 1, pix * 1, pix, pix });
	fallAnimL.PushBack({ pix * 0, pix * 1, pix, pix });
	fallAnimL.loop = false;
	fallAnimL.speed = 0.15f;

	// walk
	walkAnimR.PushBack({ pix * 0, pix * 5, pix, pix });
	walkAnimR.PushBack({ pix * 1, pix * 5, pix, pix });
	walkAnimR.PushBack({ pix * 2, pix * 5, pix, pix });
	walkAnimR.PushBack({ pix * 3, pix * 5, pix, pix });
	walkAnimR.PushBack({ pix * 4, pix * 5, pix, pix });
	walkAnimR.PushBack({ pix * 5, pix * 5, pix, pix });
	walkAnimR.PushBack({ pix * 6, pix * 5, pix, pix });
	walkAnimR.PushBack({ pix * 7, pix * 5, pix, pix });
	walkAnimR.speed = 0.25f;

	walkAnimL.PushBack({ pix * 7, pix * 5, pix, pix });
	walkAnimL.PushBack({ pix * 6, pix * 5, pix, pix });
	walkAnimL.PushBack({ pix * 5, pix * 5, pix, pix });
	walkAnimL.PushBack({ pix * 4, pix * 5, pix, pix });
	walkAnimL.PushBack({ pix * 3, pix * 5, pix, pix });
	walkAnimL.PushBack({ pix * 2, pix * 5, pix, pix });
	walkAnimL.PushBack({ pix * 1, pix * 5, pix, pix });
	walkAnimL.PushBack({ pix * 0, pix * 5, pix, pix });
	walkAnimL.speed = 0.25f;

	// death
	deathAnimR.PushBack({ pix * 0, pix * 0, pix, pix });
	deathAnimR.PushBack({ pix * 1, pix * 0, pix, pix });
	deathAnimR.PushBack({ pix * 2, pix * 0, pix, pix });
	deathAnimR.PushBack({ pix * 3, pix * 0, pix, pix });
	deathAnimR.PushBack({ pix * 4, pix * 0, pix, pix });
	deathAnimR.PushBack({ pix * 5, pix * 0, pix, pix });
	deathAnimR.PushBack({ pix * 6, pix * 0, pix, pix });
	deathAnimR.PushBack({ pix * 7, pix * 0, pix, pix });
	deathAnimR.PushBack({ pix * 8, pix * 0, pix, pix });
	deathAnimR.PushBack({ pix * 9, pix * 0, pix, pix });
	deathAnimR.loop = false;
	deathAnimR.speed = 0.05f;

	deathAnimL.PushBack({ pix * 9, pix * 0, pix, pix });
	deathAnimL.PushBack({ pix * 8, pix * 0, pix, pix });
	deathAnimL.PushBack({ pix * 7, pix * 0, pix, pix });
	deathAnimL.PushBack({ pix * 6, pix * 0, pix, pix });
	deathAnimL.PushBack({ pix * 5, pix * 0, pix, pix });
	deathAnimL.PushBack({ pix * 4, pix * 0, pix, pix });
	deathAnimL.PushBack({ pix * 3, pix * 0, pix, pix });
	deathAnimL.PushBack({ pix * 2, pix * 0, pix, pix });
	deathAnimL.PushBack({ pix * 1, pix * 0, pix, pix });
	deathAnimL.PushBack({ pix * 0, pix * 0, pix, pix });
	deathAnimL.loop = false;
	deathAnimL.speed = 0.05f;

	pix = 109;

	readyAnim.PushBack({ pix * 0, pix * 0, pix, pix });
	readyAnim.loop = false;

	chargeAnim.PushBack({ pix * 1, pix * 0, pix, pix });
	chargeAnim.PushBack({ pix * 2, pix * 0, pix, pix });
	chargeAnim.PushBack({ pix * 3, pix * 0, pix, pix });
	chargeAnim.PushBack({ pix * 4, pix * 0, pix, pix });
	chargeAnim.loop = false;

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
	textureR = app->tex->Load("Assets/textures/Player.png");
	textureL = app->tex->Load("Assets/textures/PlayerInv.png");

	currentAnimation = &idleAnimR;
	lookLeft = false;

	walk_sound = app->audio->LoadFx("Assets/audio/fx/step_sound.wav");
	jump_sound = app->audio->LoadFx("Assets/audio/fx/jump_sound.wav");

	// player Save Data
	nlifes = 2;
	sprintf_s(numLifes, 3, "%02d", nlifes);

	// player
	b2BodyDef p_body;
	p_body.type = b2_dynamicBody;
	p_body.fixedRotation = true;
	p_body.position.Set(x, y);
	
	player_body = app->physics->world->CreateBody(&p_body);
	player_body->SetBullet(true);
	player_body->SetFixedRotation(true);

	b2PolygonShape box;
	box.SetAsBox(PIXELS_TO_METERS(w), PIXELS_TO_METERS(h));

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.friction = 0.0f;
	b2Fixture* bodyFixture = player_body->CreateFixture(&fixture);
	bodyFixture->SetSensor(false);
	bodyFixture->SetUserData((void*)1); // player collision

	// ground sensor
	box.SetAsBox(PIXELS_TO_METERS((w - 2)), PIXELS_TO_METERS(2), b2Vec2(0, PIXELS_TO_METERS(33)), 0);
	fixture.isSensor = true;
	b2Fixture* sensorFixture = player_body->CreateFixture(&fixture);
	sensorFixture->SetUserData((void*)2); // ground sensor

	hab_tex = app->tex->Load("Assets/textures/HabilityMack.png");

	habAnimation = &readyAnim;
	hab_ready = true;
	hab_cd = 60;
	dashing = false;
	dash_time = 3;

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
	float fixedSpeed = speed * dt;
	float fixedJumpForce = jumpForce * dt;

	if (!app->menu->GetGameState() && (currentAnimation != &deathAnimL &&
		currentAnimation != &deathAnimR) && !app->scene->GetStartScreenState())
	{
		if (!dashing)
		{
			// move left
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				player_body->SetLinearVelocity({ -fixedSpeed, player_body->GetLinearVelocity().y });
				lookLeft = true;

				if (currentAnimation != &walkAnimL && !inAir)
				{
					walkAnimL.Reset();
					currentAnimation = &walkAnimL;
				}

				//app->audio->PlayFx(walk_sound);
			}
			else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
			{
				player_body->SetLinearVelocity({ 0, player_body->GetLinearVelocity().y });

				if (currentAnimation != &idleAnimL && !inAir)
				{
					idleAnimL.Reset();
					currentAnimation = &idleAnimL;
				}
			}

			//move right
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				player_body->SetLinearVelocity({ fixedSpeed, player_body->GetLinearVelocity().y });
				lookLeft = false;

				if (currentAnimation != &walkAnimR && !inAir)
				{
					walkAnimR.Reset();
					currentAnimation = &walkAnimR;
				}
			}
			else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
			{
				player_body->SetLinearVelocity({ 0, player_body->GetLinearVelocity().y });

				if (currentAnimation != &idleAnimR && !inAir)
				{
					idleAnimR.Reset();
					currentAnimation = &idleAnimR;
				}
			}

			if (!inAir && player_body->GetLinearVelocity().x != 0)
			{
				walk_cd--;
				if (walk_cd <= 0)
				{
					app->audio->PlayFx(walk_sound);
					walk_cd = 20;
				}
			}

			//jump
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				if (!inAir)
				{
					player_body->SetLinearVelocity({ player_body->GetLinearVelocity().x , 0 });
					player_body->ApplyForceToCenter({ 0, -fixedJumpForce }, true);
					app->audio->PlayFx(jump_sound);
					inAir = true;
				}
				else if (djump)
				{
					player_body->SetLinearVelocity({ player_body->GetLinearVelocity().x , 0 });
					player_body->ApplyForceToCenter({ 0, -fixedJumpForce }, true);
					app->audio->PlayFx(jump_sound);
					djump = false;
				}
			}

			if (player_body->GetLinearVelocity().y < 0 && inAir)
			{
				if (lookLeft)
				{
					if (currentAnimation != &jumpAnimL)
					{
						jumpAnimL.Reset();
						currentAnimation = &jumpAnimL;
					}
				}
				else
				{
					if (currentAnimation != &jumpAnimR)
					{
						jumpAnimR.Reset();
						currentAnimation = &jumpAnimR;
					}
				}

			}
			else if (player_body->GetLinearVelocity().y > 0 && inAir)
			{
				if (lookLeft)
				{
					if (currentAnimation != &fallAnimL)
					{
						fallAnimL.Reset();
						currentAnimation = &fallAnimL;
					}
				}
				else
				{
					if (currentAnimation != &fallAnimR)
					{
						fallAnimR.Reset();
						currentAnimation = &fallAnimR;
					}
				}
			}
			else if (player_body->GetLinearVelocity().x == 0 && player_body->GetLinearVelocity().y == 0)
			{
				if (lookLeft)
				{
					if (currentAnimation != &idleAnimL)
					{
						idleAnimL.Reset();
						currentAnimation = &idleAnimL;
					}
				}
				else
				{
					if (currentAnimation != &idleAnimR)
					{
						idleAnimR.Reset();
						currentAnimation = &idleAnimR;
					}
				}
			}

			currentAnimation->Update();
		}
	}
		

	if (currentAnimation == &deathAnimL || currentAnimation == &deathAnimR)
	{
		player_body->SetLinearVelocity({ 0 , 0 });
		currentAnimation->Update();

		death_cd--;
		if (death_cd <= 0)
		{
			// death screen
			if(nlifes <= 0)
			{
				app->menu->lose = true;
			}
			else
			{
				app->menu->dead = true;

			}
		}
	}
	else
	{
		death_cd = 120;
	}

	//hability
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN)
	{
		if (hab_ready)
		{
			dashing = true;

			// cast
			if (lookLeft)
			{
				player_body->ApplyForceToCenter({ -2000 * speed * dt, 0 }, true);
			}
			else
			{
				player_body->ApplyForceToCenter({ 2000 * speed * dt, 0 }, true);
			}

			if (habAnimation != &chargeAnim)
			{
				chargeAnim.Reset();
				habAnimation = &chargeAnim;
			}
			
			hab_ready = false;

		}
	}

	if (dashing)
	{
		dash_time--;

		if (dash_time <= 0)
		{
			dashing = false;
			dash_time = 3;
			player_body->SetLinearVelocity({ 0, 0 });
		}
	}

	if (!hab_ready)
	{
		hab_cd--;

		if (hab_cd <= 0)
		{
			chargeAnim.Update();
			hab_cd = 60;
		}

		if (chargeAnim.HasFinished())
		{
			hab_ready = true;
			habAnimation = &readyAnim;
			chargeAnim.loopCount = 0;
		}
	}

	return true;
}

// Called each loop iteration
bool Player::PostUpdate()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	 
	if (!app->scene->GetStartScreenState())
	{
		if (lookLeft)
		{
			app->render->DrawTexture(textureL, METERS_TO_PIXELS(x - 55.5f), METERS_TO_PIXELS(y - 70.0f), &rect);
		}
		else
		{
			app->render->DrawTexture(textureR, METERS_TO_PIXELS(x - 55.5f), METERS_TO_PIXELS(y - 70.0f), &rect);
		}
	}
	
	return true;
}

// Called before quitting
bool Player::CleanUp()
{

	return true;
}

bool Player::LoadState(pugi::xml_node& data)
{
	x = data.child("position").attribute("x").as_int();
	y = data.child("position").attribute("y").as_int();

	player_body->SetTransform({ x + PIXELS_TO_METERS(w), y }, player_body->GetAngle());
	player_body->ApplyForceToCenter({ 0, 200 }, true);

	currentAnimation = &idleAnimR;
	if (app->menu->dead)
	{
		app->menu->dead = false;
	}

	return true;
}

bool Player::SaveState(pugi::xml_node& data)
{
	data.child("position").attribute("x").set_value(x);
	data.child("position").attribute("y").set_value(y);

	return true;
}

bool Player::Death()
{
	nlifes--;
	sprintf_s(numLifes, 3, "%02d", nlifes);

	if (lookLeft)
	{
		deathAnimL.Reset();
		currentAnimation = &deathAnimL;
	}
	else
	{
		deathAnimR.Reset();
		currentAnimation = &deathAnimR;
	}

	return true;
}

fPoint Player::GetPosition()
{
	return { x, y };
}

void Player::SetPosition(int new_x, int new_y)
{
	x = new_x;
	y = new_y;

	player_body->SetTransform({ x, y }, player_body->GetAngle());
	player_body->ApplyForceToCenter({ 0, 50 }, true);
}