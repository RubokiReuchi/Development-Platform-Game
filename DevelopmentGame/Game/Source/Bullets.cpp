#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Physics.h"
#include "Bullets.h"

#include "Defs.h"
#include "Log.h"

Bullets::Bullets() : Module()
{
	name.Create("bullets");
	int pix = 32;

	flyAnimR.PushBack({ pix * 0, pix * 0, pix, pix });
	flyAnimR.PushBack({ pix * 1, pix * 0, pix, pix });
	flyAnimR.PushBack({ pix * 2, pix * 0, pix, pix });
	flyAnimR.PushBack({ pix * 3, pix * 0, pix, pix });
	flyAnimR.speed = 0.25f;

	flyAnimL.PushBack({ pix * 4, pix * 0, pix, pix });
	flyAnimL.PushBack({ pix * 5, pix * 0, pix, pix });
	flyAnimL.PushBack({ pix * 6, pix * 0, pix, pix });
	flyAnimL.PushBack({ pix * 7, pix * 0, pix, pix });
	flyAnimL.speed = 0.25f;

	exploteAnim.PushBack({ pix * 1, pix * 1, pix, pix });
	exploteAnim.PushBack({ pix * 2, pix * 1, pix, pix });
	exploteAnim.PushBack({ pix * 3, pix * 1, pix, pix });
	exploteAnim.PushBack({ pix * 4, pix * 1, pix, pix });
	exploteAnim.PushBack({ pix * 5, pix * 1, pix, pix });
	exploteAnim.PushBack({ pix * 6, pix * 1, pix, pix });
	exploteAnim.PushBack({ pix * 7, pix * 1, pix, pix });
	exploteAnim.speed = 0.25f;
	exploteAnim.loop = false;
}

// Destructor
Bullets::~Bullets()
{}

// Called before render is available
bool Bullets::Awake()
{
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Bullets::Start()
{
	tex = app->tex->Load("Assets/textures/Shoot.png");

	currentAnimation = &flyAnimR;

	return true;
}

// Called each loop iteration
bool Bullets::PreUpdate()
{
	if (lifeTime > 0)
	{
		lifeTime--;
	}

	return true;
}

// Called each loop iteration
bool Bullets::Update(float dt)
{
	if (lifeTime <= 0)
	{
		app->physics->world->DestroyBody(body);
	}

	return true;
}

// Called each loop iteration
bool Bullets::PostUpdate()
{
	bool ret = true;


	return ret;
}

// Called before quitting
bool Bullets::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void Bullets::SpawnBullet(int x, int y, int direction)
{
	b2BodyDef b_body;
	b_body.type = b2_dynamicBody;
	b_body.position.Set(x, y);

	body = app->physics->world->CreateBody(&b_body);
	body->SetBullet(true);

	b2CircleShape circle;
	circle.m_p.Set(x, y);
	circle.m_radius = 0.5f;

	b2FixtureDef fixture;
	fixture.shape = &circle;
	fixture.density = 0.0f;
	fixture.friction = 0.0f;
	b2Fixture* bodyFixture = body->CreateFixture(&fixture);
	bodyFixture->SetSensor(true);
	bodyFixture->SetUserData((void*)8);

	body->SetLinearVelocity({ speed * direction, 0 });

	if (direction == 1)
	{
		if (currentAnimation != &flyAnimR)
		{
			flyAnimR.Reset();
			currentAnimation = &flyAnimR;
		}
	}
	else
	{
		if (currentAnimation != &flyAnimL)
		{
			flyAnimL.Reset();
			currentAnimation = &flyAnimL;
		}
	}
	
}