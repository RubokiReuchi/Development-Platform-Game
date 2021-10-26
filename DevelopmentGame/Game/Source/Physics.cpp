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

#ifdef _DEBUG
#pragma comment( lib, "Source/External/Box2D/libx86/Release/Box2D.lib" )
#else
#pragma comment( lib, "Source/External/Box2D/libx86/Release/Box2D.lib" )
#endif

Physics::Physics() : Module()
{
	name.Create("physics");
	debug = true;
}

// Destructor
Physics::~Physics()
{}

// Called before render is available
bool Physics::Awake()
{

	return true;
}

// Called before the first frame
bool Physics::Start()
{
	b2Vec2 gravity(GRAVITY_X, -GRAVITY_Y);
	world = new b2World(gravity);
	world->SetContactListener(this);

	on_collosion = 0;

	return true;
}

// Called each loop iteration
bool Physics::PreUpdate()
{
	if (!app->menu->GetGameState())
	{
		world->Step(1.0f / 60.0f, 6, 2);
	}
	else
	{
		world->Step(0, 6, 2);
	}
	
	if (on_collosion > 0)
	{
		app->player->inAir = false;
		app->player->djump = true;
	}
	else
	{
		app->player->inAir = true;
	}

	return true;
}

// Called each loop iteration
bool Physics::Update(float dt)
{

	return true;
}

// Called each loop iteration
bool Physics::PostUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (!debug)
		return true;

	Uint8 c_r, c_g, c_b;
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
			int32 count = polygonShape->GetVertexCount();
			b2Vec2 prev, v;

			for (int32 i = 0; i < count; ++i)
			{
				v = b->GetWorldPoint(polygonShape->GetVertex(i));
				if (i > 0)
				{
					void* userData = f->GetUserData();
					
					switch ((int)userData)
					{
					case 0:
						c_r = 0;
						c_g = 0;
						c_b = 255;
						break;
					case 1:
						c_r = 255;
						c_g = 128;
						c_b = 0;
						break;
					case 2:
						c_r = 128;
						c_g = 0;
						c_b = 255;
						break;
					case 3:
						c_r = 0;
						c_g = 255;
						c_b = 0;
						break;
					case 4:
						c_r = 255;
						c_g = 0;
						c_b = 0;
						break;
					case 5:
						c_r = 255;
						c_g = 200;
						c_b = 0;
						break;
					default:
						c_r = 0;
						c_g = 0;
						c_b = 255;
						break;
					}
					app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), c_r, c_g, c_b);
				}

				prev = v;
			}

			v = b->GetWorldPoint(polygonShape->GetVertex(0));
			app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), c_r, c_g, c_b);
		}
	}

	return true;
}

// Called before quitting
bool Physics::CleanUp()
{

	return true;
}

bool Physics::CreateBox(int x, int y, int w, int h, int collision)
{
	b2BodyDef g;
	g.type = b2_staticBody;
	g.position.Set(PIXELS_TO_METERS(x), PIXELS_TO_METERS(y));

	b2Body* p = world->CreateBody(&g);

	b2PolygonShape box;
	box.SetAsBox(PIXELS_TO_METERS(w), PIXELS_TO_METERS(h));

	b2FixtureDef fixture;
	fixture.shape = &box;
	if (collision == 5) fixture.isSensor = true;
	b2Fixture* fix = p->CreateFixture(&fixture);

	fix->SetUserData((void*)collision);

	return true;
}

void Physics::BeginContact(b2Contact* contact)
{
	void* fixtureUserDataA = contact->GetFixtureA()->GetUserData();
	void* fixtureUserDataB = contact->GetFixtureB()->GetUserData();

	if ((int)fixtureUserDataA == 2)
	{
		if ((int)fixtureUserDataB == 3)
		{
			on_collosion++;
		}
		else if ((int)fixtureUserDataB == 4)
		{
			// player death
			app->player->Death();
		}
		else if ((int)fixtureUserDataB == 5)
		{
			// show complete level key
		}
	}

	if ((int)fixtureUserDataB == 2)
	{
		if ((int)fixtureUserDataA == 3)
		{
			on_collosion++;
		}
		else if ((int)fixtureUserDataA == 4)
		{
			// player death
			app->player->Death();
		}
		else if ((int)fixtureUserDataA == 5)
		{
			// show complete level key
		}
	}
}

void Physics::EndContact(b2Contact* contact)
{
	void* fixtureUserDataA = contact->GetFixtureA()->GetUserData();
	void* fixtureUserDataB = contact->GetFixtureB()->GetUserData();

	if ((int)fixtureUserDataA == 2)
	{
		if ((int)fixtureUserDataB == 3)
		{
			on_collosion--;
		}
		else if ((int)fixtureUserDataB == 5)
		{
			// hide complete level key
		}
	}

	if ((int)fixtureUserDataB == 2)
	{
		if ((int)fixtureUserDataA == 3)
		{
			on_collosion--;
		}
		else if ((int)fixtureUserDataA == 5)
		{
			// hide complete level key
		}
	}
}