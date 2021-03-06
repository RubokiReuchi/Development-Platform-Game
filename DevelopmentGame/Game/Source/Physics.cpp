#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Physics.h"
#include "Entities.h"
#include "Player.h"
#include "Enemies.h"
#include "Coins.h"
#include "Hearts.h"
#include "Menu.h"
#include "Frontground.h"

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
	debug = false;
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

	save_sound = app->audio->LoadFx("Assets/audio/fx/save_sound.wav");;
	water_well_sound = app->audio->LoadFx("Assets/audio/fx/water_well_sound.wav");;
	level_complete_sound = app->audio->LoadFx("Assets/audio/fx/level_complete_sound.wav");
	death_sound = app->audio->LoadFx("Assets/audio/fx/death_sound.wav");

	return true;
}

// Called each loop iteration
bool Physics::PreUpdate()
{
	if (!app->menu->GetGameState() && !app->scene->GetStartScreenState())
	{
		world->Step(1.0f / 60.0f, 6, 2);
	}
	else
	{
		world->Step(0, 6, 2);
	}

	return true;
}

// Called each loop iteration
bool Physics::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		debug = !debug;
	}

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		if (inScareCrow)
		{
			app->SaveGameRequest();
			app->audio->PlayFx(save_sound);
		}
		else if (inStatue)
		{
			app->scene->PassLevel(app->scene->current_level + 1);
			app->frontground->SetPressE_Hide(true);
			app->audio->PlayFx(level_complete_sound);
			inStatue = false;
		}
		else if (inWaterWell)
		{
			app->audio->PlayFx(water_well_sound);
			app->frontground->SetPressE_Hide(true);
			inWaterWell = false;
		}
	}

	return true;
}

// Called each loop iteration
bool Physics::PostUpdate()
{
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
					case 6:
						c_r = 200;
						c_g = 200;
						c_b = 200;
						break;
					case 7:
						c_r = 100;
						c_g = 100;
						c_b = 100;
						break;
					case 8:
						c_r = 50;
						c_g = 200;
						c_b = 200;
						break;
					case 9:
						c_r = 50;
						c_g = 50;
						c_b = 50;
						break;
					case 10:
						c_r = 100;
						c_g = 100;
						c_b = 100;
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

bool Physics::CreateMapBox(int x, int y, int w, int h, int collision)
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
	else if (collision == 6) fixture.isSensor = true;
	else if (collision == 7) fixture.isSensor = true;
	b2Fixture* fix = p->CreateFixture(&fixture);

	fix->SetUserData((void*)collision);

	return true;
}

bool Physics::CleanMapBoxes()
{
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			if ((int)f->GetUserData() >= 3)
			{
				b->DestroyFixture(f);
				world->DestroyBody(b);
			}
		}
	}

	return true;
}

void Physics::BeginContact(b2Contact* contact)
{
	void* fixtureUserDataA = contact->GetFixtureA()->GetUserData();
	void* fixtureUserDataB = contact->GetFixtureB()->GetUserData();

	if ((int)fixtureUserDataA == 4)
	{
		if ((int)fixtureUserDataB == 4)
		{
			ListItem<Entity*>* item;
			Entity* entity = NULL;
			Ground_Enemies* g = NULL;

			for (item = app->entities->entities.start; item != NULL; item = item->next)
			{
				entity = item->data;

				if (entity->entity_type == ENTITY_TYPE::GROUND_ENEMY)
				{
					entity->SwitchDirection();
				}
			}
		}
	}
	else if ((int)fixtureUserDataA == 2)
	{
		if ((int)fixtureUserDataB == 3)
		{
			on_collosion++;
		}
		else if ((int)fixtureUserDataB == 9)
		{
			Entity* entity = app->entities->GetPlayer();
			app->entities->KillEnemy(entity->GetPlayerPosition());
		}
	}
	else if ((int)fixtureUserDataA == 1)
	{
		if ((int)fixtureUserDataB == 4 && !app->scene->godmode)
		{
			// player death
			Entity* entity = app->entities->GetPlayer();
			entity->PlayerDeath();
			app->audio->PlayFx(death_sound);
		}
		else if ((int)fixtureUserDataB == 5)
		{
			// save level
			app->frontground->SetPressE_Hide(false);
			inScareCrow = true;
		}
		else if ((int)fixtureUserDataB == 6)
		{
			// complete level
			if (!statueUsed)
			{
				app->frontground->SetPressE_Hide(false);
				inStatue = true;
			}
		}
		else if ((int)fixtureUserDataB == 7)
		{
			// water well
			app->frontground->SetPressE_Hide(false);
			inWaterWell = true;
		}
		else if ((int)fixtureUserDataB == 8)
		{
			// coin
			Entity* entity = app->entities->GetPlayer();
			app->entities->PickCoin(entity->GetPlayerPosition());
		}
		else if ((int)fixtureUserDataB == 10)
		{
			// hearts
			Entity* entity = app->entities->GetPlayer();
			app->entities->PickHeart(entity->GetPlayerPosition());
		}
	}

	if ((int)fixtureUserDataB == 2)
	{
		if ((int)fixtureUserDataA == 3)
		{
			on_collosion++;
		}
		else if ((int)fixtureUserDataA == 9)
		{
			Entity* entity = app->entities->GetPlayer();
			app->entities->KillEnemy(entity->GetPlayerPosition());
		}
	}
	else if ((int)fixtureUserDataB == 1)
	{
		if ((int)fixtureUserDataA == 4 && !app->scene->godmode)
		{
			// player death
			Entity* entity = app->entities->GetPlayer();
			entity->PlayerDeath();
			app->audio->PlayFx(death_sound);
		}
		else if ((int)fixtureUserDataA == 5)
		{
			// save level
			app->frontground->SetPressE_Hide(false);
			inScareCrow = true;
		}
		else if ((int)fixtureUserDataA == 6)
		{
			// complete level
			if (!statueUsed)
			{
				app->frontground->SetPressE_Hide(false);
				inStatue = true;
			}
		}
		else if ((int)fixtureUserDataA == 7)
		{
			// water well
			app->frontground->SetPressE_Hide(false);
			inWaterWell = true;
		}
		else if ((int)fixtureUserDataA == 8)
		{
			// coin
			Entity* entity = app->entities->GetPlayer();
			app->entities->PickCoin(entity->GetPlayerPosition());
		}
		else if ((int)fixtureUserDataA == 10)
		{
			// hearts
			Entity* entity = app->entities->GetPlayer();
			app->entities->PickHeart(entity->GetPlayerPosition());
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
	}
	else if ((int)fixtureUserDataA == 1)
	{
		if ((int)fixtureUserDataB == 5)
		{
			// hide save level
			app->frontground->SetPressE_Hide(true);
			inScareCrow = false;
		}
		else if ((int)fixtureUserDataB == 6)
		{
			//  hide complete level
			app->frontground->SetPressE_Hide(true);
			inStatue = false;
		}
		else if ((int)fixtureUserDataB == 7)
		{
			// hide water well
			app->frontground->SetPressE_Hide(true);
			inWaterWell = false;
		}
	}

	if ((int)fixtureUserDataB == 2)
	{
		if ((int)fixtureUserDataA == 3)
		{
			on_collosion--;
		}
	}
	else if ((int)fixtureUserDataB == 1)
	{
		if ((int)fixtureUserDataA == 5)
		{
			// hide save level
			app->frontground->SetPressE_Hide(true);
			inScareCrow = false;
		}
		else if ((int)fixtureUserDataA == 6)
		{
			// hide complete level
			app->frontground->SetPressE_Hide(true);
			inStatue = false;
		}
		else if ((int)fixtureUserDataA == 7)
		{
			// hide water well
			app->frontground->SetPressE_Hide(true);
			inWaterWell = false;
		}
	}
}