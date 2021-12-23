#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Map.h"
#include "Hearts.h"
#include "Player.h"
#include "Defs.h"
#include "Log.h"

Hearts::Hearts() : Module()
{
	name.Create("hearts");

}

// Destructor
Hearts::~Hearts()
{}

// Called before render is available
bool Hearts::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Hearts::Start()
{
	texture = app->tex->Load("Assets/textures/Heart.png");

	nlifes = 2;
	sprintf_s(numLifes, 3, "%02d", nlifes);
	
	return true;
}

// Called each loop iteration
bool Hearts::PreUpdate()
{
	for (size_t i = 0; i < hearts.Count(); i++)
	{
		if (!hearts.At(i)->picked)
		{
			hearts.At(i)->x = hearts.At(i)->body->GetPosition().x;
			hearts.At(i)->y = hearts.At(i)->body->GetPosition().y;
		}
	}

	return true;
}

// Called each loop iteration
bool Hearts::Update(float dt)
{
	return true;
}

// Called each loop iteration
bool Hearts::PostUpdate()
{
	bool ret = true;

	for (size_t i = 0; i < hearts.Count(); i++)
	{
		if (hearts.At(i)->plan_to_delete)
		{
			app->physics->world->DestroyBody(hearts.At(i)->body);
			hearts.At(i)->plan_to_delete = false;

		}
		
		if (!hearts.At(i)->picked)
		{
			app->render->DrawTexture(texture, METERS_TO_PIXELS(hearts.At(i)->x), METERS_TO_PIXELS(hearts.At(i)->y));
		}
		
	}

	return ret;
}

// Called before quitting
bool Hearts::CleanUp()
{
	LOG("Freeing scene");

	return true;
}



void Hearts::CreateHeart(int x, int y)
{
	// heart
	Heart* new_heart = new Heart();

	b2BodyDef c_body;
	c_body.type = b2_staticBody;
	c_body.position.Set(PIXELS_TO_METERS(x), PIXELS_TO_METERS(y));

	new_heart->body = app->physics->world->CreateBody(&c_body);

	b2PolygonShape box;
	box.SetAsBox(PIXELS_TO_METERS(w), PIXELS_TO_METERS(h), b2Vec2(PIXELS_TO_METERS(16), PIXELS_TO_METERS(16)), 0);

	b2FixtureDef fixture;
	fixture.shape = &box;
	b2Fixture* bodyFixture = new_heart->body->CreateFixture(&fixture);
	bodyFixture->SetSensor(true);
	bodyFixture->SetUserData((void*)10); // heart collision

	new_heart->picked = false;

	hearts.Insert(*new_heart, hearts.Count());
}


void Hearts::PickHeart(float x, float y)
{
	for (size_t i = 0; i < hearts.Count(); i++)
	{
		if (x + 1.5f > hearts.At(i)->x && x - 1.5f < hearts.At(i)->x && y + 2.0f > hearts.At(i)->y && y - 2.0f < hearts.At(i)->y)
		{
			if (!hearts.At(i)->picked)
			{
				nlifes++;
				sprintf_s(numLifes, 3, "%02d", nlifes);

				hearts.At(i)->picked = true;
				hearts.At(i)->plan_to_delete = true;
			}
			
			break;
		}
	}
}