#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Physics.h"

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

	b2BodyDef g;
	g.type = b2_staticBody;
	g.position.Set(PIXEL_TO_METERS(400), PIXEL_TO_METERS(500));

	b2Body* p = world->CreateBody(&g);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(250), PIXEL_TO_METERS(32));

	b2FixtureDef fixture;
	fixture.shape = &box;
	p->CreateFixture(&fixture);

	b2BodyDef w;
	w.type = b2_staticBody;
	w.position.Set(PIXEL_TO_METERS(400), PIXEL_TO_METERS(500));

	b2Body* q = world->CreateBody(&w);

	b2PolygonShape box2;
	box2.SetAsBox(PIXEL_TO_METERS(32), PIXEL_TO_METERS(250));

	b2FixtureDef q_fixture;
	q_fixture.shape = &box2;
	q->CreateFixture(&q_fixture);

	return true;
}

// Called each loop iteration
bool Physics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

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
					app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 0, 0);

				prev = v;
			}

			v = b->GetWorldPoint(polygonShape->GetVertex(0));
			app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 0, 0);
		}
	}

	return true;
}

// Called before quitting
bool Physics::CleanUp()
{

	return true;
}