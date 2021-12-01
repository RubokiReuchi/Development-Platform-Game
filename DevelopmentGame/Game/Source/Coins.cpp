#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Map.h"
#include "Coins.h"

#include "Defs.h"
#include "Log.h"

Coins::Coins() : Module()
{
	name.Create("scene");

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
		if (!coins.At(i)->picked)
		{
			SDL_Rect rect = currentAnimation->GetCurrentFrame();
			
			app->render->DrawTexture(texture, coins.At(i)->x, coins.At(i)->y, &rect);
		}
	}

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
	c_body.position.Set(x, y);

	new_coin->body = app->physics->world->CreateBody(&c_body);

	b2PolygonShape box;
	box.SetAsBox(PIXELS_TO_METERS(w), PIXELS_TO_METERS(h));

	b2FixtureDef fixture;
	fixture.shape = &box;
	b2Fixture* bodyFixture = new_coin->body->CreateFixture(&fixture);
	bodyFixture->SetSensor(true);
	bodyFixture->SetUserData((void*)8); // coin collision

	new_coin->x = x;
	new_coin->y = y;

	new_coin->picked = false;

	coins.Insert(*new_coin, coins.Count());
}