#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Map.h"
#include "Coins.h"
#include "Defs.h"
#include "Log.h"

Coins::Coins() : Module()
{
	name.Create("coins");

	int pix = 32;

	// idle animation
	idleAnim.PushBack({ pix * 0, pix * 0, pix, pix });
	idleAnim.PushBack({ pix * 1, pix * 0, pix, pix });
	idleAnim.PushBack({ pix * 2, pix * 0, pix, pix });
	idleAnim.PushBack({ pix * 3, pix * 0, pix, pix });
	idleAnim.PushBack({ pix * 4, pix * 0, pix, pix });
	idleAnim.speed = 0.15f;
}

// Destructor
Coins::~Coins()
{}

// Called before render is available
bool Coins::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Coins::Start()
{
	texture = app->tex->Load("Assets/textures/Coin.png");

	currentAnimation = &idleAnim;
	
	return true;
}

// Called each loop iteration
bool Coins::PreUpdate()
{
	for (size_t i = 0; i < coins.Count(); i++)
	{
		if (!coins.At(i)->picked)
		{
			coins.At(i)->x = coins.At(i)->body->GetPosition().x;
			coins.At(i)->y = coins.At(i)->body->GetPosition().y;
		}
	}

	return true;
}

// Called each loop iteration
bool Coins::Update(float dt)
{
	currentAnimation->Update();
	
	return true;
}

// Called each loop iteration
bool Coins::PostUpdate()
{
	bool ret = true;

	for (size_t i = 0; i < coins.Count(); i++)
	{
		if (coins.At(i)->plan_to_delete)
		{
			app->physics->world->DestroyBody(coins.At(i)->body);
			coins.At(i)->plan_to_delete = false;

		}

		if (!coins.At(i)->picked)
		{
			SDL_Rect rect = currentAnimation->GetCurrentFrame();
			app->render->DrawTexture(texture, METERS_TO_PIXELS(coins.At(i)->x), METERS_TO_PIXELS(coins.At(i)->y), &rect);
		}
		
	}


	sprintf_s(numCoins, 4, "%03d", ncoins);
	return ret;
}

// Called before quitting
bool Coins::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void Coins::CreateCoin(int x, int y)
{
	// coin
	Coin* new_coin = new Coin();

	b2BodyDef c_body;
	c_body.type = b2_staticBody;
	c_body.position.Set(PIXELS_TO_METERS(x), PIXELS_TO_METERS(y));

	new_coin->body = app->physics->world->CreateBody(&c_body);

	b2PolygonShape box;
	box.SetAsBox(PIXELS_TO_METERS(w), PIXELS_TO_METERS(h), b2Vec2(PIXELS_TO_METERS(16), PIXELS_TO_METERS(16)), 0);

	b2FixtureDef fixture;
	fixture.shape = &box;
	b2Fixture* bodyFixture = new_coin->body->CreateFixture(&fixture);
	bodyFixture->SetSensor(true);
	bodyFixture->SetUserData((void*)8); // coin collision

	new_coin->picked = false;

	coins.Insert(*new_coin, coins.Count());
}

void Coins::PickCoin(float x, float y)
{
	for (size_t i = 0; i < coins.Count(); i++)
	{
		if (x + 1.5f > coins.At(i)->x && x - 1.5f < coins.At(i)->x && y + 2.0f > coins.At(i)->y && y - 2.0f < coins.At(i)->y)
		{
			if (!coins.At(i)->picked)
			{
				ncoins++;
				sprintf_s(numCoins, 4, "%03d", ncoins);
				coins.At(i)->picked = true;
				coins.At(i)->plan_to_delete = true;
			}
			
			break;
		}
	}
}