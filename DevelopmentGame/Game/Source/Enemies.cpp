#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Physics.h"
#include "Player.h"
#include "Enemies.h"
#include "Pathfinding.h"
#include "Menu.h"

#include "Defs.h"
#include "Log.h"

#include <time.h>

// GROUND ENEMY
Ground_Enemies::Ground_Enemies() : Entity()
{
	int pix = 64;

	// walk slime
	slime_walkAnimR.PushBack({ pix * 0, pix * 0, pix, pix });
	slime_walkAnimR.PushBack({ pix * 1, pix * 0, pix, pix });
	slime_walkAnimR.PushBack({ pix * 2, pix * 0, pix, pix });
	slime_walkAnimR.PushBack({ pix * 3, pix * 0, pix, pix });
	slime_walkAnimR.speed = 0.02f;
	slime_walkAnimR.pingpong = true;

	slime_walkAnimL.PushBack({ pix * 0, pix * 0, pix, pix });
	slime_walkAnimL.PushBack({ pix * 1, pix * 0, pix, pix });
	slime_walkAnimL.PushBack({ pix * 2, pix * 0, pix, pix });
	slime_walkAnimL.PushBack({ pix * 3, pix * 0, pix, pix });
	slime_walkAnimL.speed = 0.02f;
	slime_walkAnimL.pingpong = true;
	
}

// Destructor
Ground_Enemies::~Ground_Enemies()
{}

// Called each loop iteration
void Ground_Enemies::PreUpdate()
{
	if (state != ENEMY_STATE::DEATH)
	{
		position.x = body->GetPosition().x;
		position.y = body->GetPosition().y;

		if (obLeft)
		{
			idleOb_x = origin_x - PIXELS_TO_METERS(32 * 5);
		}
		else
		{
			idleOb_x = origin_x + PIXELS_TO_METERS(32 * 5);
		}
	}
}

// Called each loop iteration
void Ground_Enemies::Update(float dt)
{
	currentAnimation->Update();

	// update path
	switch (state)
	{
	case ENEMY_STATE::IDLE:
		MoveGroundEnemy(dt);
		break;
	case ENEMY_STATE::HUNT:
		EnemyHunting(dt);
		break;
	case ENEMY_STATE::RETURN:
		EnemyReturning(dt);
		break;
	default:
		break;
	}

	if (body->GetLinearVelocity().x >= 0)
	{
		lookLeft = false;
	}
	else
	{
		lookLeft = true;
	}

	if (state != ENEMY_STATE::DEATH)
	{
		CheckPlayer();
	}
}

// Called each loop iteration
void Ground_Enemies::Draw()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	if (plan_to_delete)
	{
		app->physics->world->DestroyBody(body);
		plan_to_delete = false;
	}

	if (state != ENEMY_STATE::DEATH)
	{
		if (lookLeft)
		{
			app->render->DrawTexture(app->entities->slime_textureL, METERS_TO_PIXELS(position.x - (40)), METERS_TO_PIXELS(position.y - (45)), &rect);
		}
		else
		{
			app->render->DrawTexture(app->entities->slime_textureR, METERS_TO_PIXELS(position.x - (40)), METERS_TO_PIXELS(position.y - (45)), &rect);
		}

		if (state == ENEMY_STATE::HUNT || state == ENEMY_STATE::RETURN)
		{
			if (app->physics->debug)
			{
				app->pathfinding->DrawPath(path_save, position, ENTITY_TYPE::GROUND_ENEMY);
			}
		}
	}
}

void Ground_Enemies::CreateGroundEnemy(float x, float y)
{
	// body
	b2BodyDef e_body;
	e_body.type = b2_dynamicBody;
	e_body.fixedRotation = true;
	e_body.position.Set(PIXELS_TO_METERS(x), PIXELS_TO_METERS(y));

	body = app->physics->world->CreateBody(&e_body);
	body->SetFixedRotation(true);

	b2PolygonShape box;
	box.SetAsBox(PIXELS_TO_METERS(w), PIXELS_TO_METERS(h));

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.friction = 0.0f;
	b2Fixture* bodyFixture = body->CreateFixture(&fixture);
	bodyFixture->SetSensor(false);
	bodyFixture->SetUserData((void*)4);

	// ground sensor
	box.SetAsBox(PIXELS_TO_METERS(w), PIXELS_TO_METERS(4), b2Vec2(0, PIXELS_TO_METERS(-20)), 0);
	fixture.isSensor = true;
	b2Fixture* sensorFixture = body->CreateFixture(&fixture);
	sensorFixture->SetUserData((void*)9); // hit sensor

	// stats
	origin_x = PIXELS_TO_METERS(x);
	origin_y = PIXELS_TO_METERS(y);
	position.x = PIXELS_TO_METERS(x);
	position.y = PIXELS_TO_METERS(y);
	speed = 0.05f;

	currentAnimation = &slime_walkAnimR;

	lookLeft = true;

	detectionRange = 5.0f;
	enemy_spoted = false;

	state = ENEMY_STATE::IDLE;
	obLeft = false;
}

void Ground_Enemies::ReviveGroundEnemy()
{
	// body
	b2BodyDef e_body;
	e_body.type = b2_dynamicBody;
	e_body.fixedRotation = true;
	e_body.position.Set(PIXELS_TO_METERS(position.x), PIXELS_TO_METERS(position.y));

	body = app->physics->world->CreateBody(&e_body);
	body->SetFixedRotation(true);

	b2PolygonShape box;
	box.SetAsBox(PIXELS_TO_METERS(w), PIXELS_TO_METERS(h));

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.friction = 0.0f;
	b2Fixture* bodyFixture = body->CreateFixture(&fixture);
	bodyFixture->SetSensor(false);
	bodyFixture->SetUserData((void*)4); // player collision

	// ground sensor
	box.SetAsBox(PIXELS_TO_METERS(w), PIXELS_TO_METERS(4), b2Vec2(0, PIXELS_TO_METERS(-20)), 0);
	fixture.isSensor = true;
	b2Fixture* sensorFixture = body->CreateFixture(&fixture);
	sensorFixture->SetUserData((void*)9); // ground sensor

	state = ENEMY_STATE::IDLE;
}

void Ground_Enemies::MoveGroundEnemy(float dt)
{
	if (!obLeft)
	{
		if (position.x < idleOb_x)
		{
			body->SetLinearVelocity({ speed * dt, body->GetLinearVelocity().y });
		}
		else
		{
			obLeft = true;
		}

	}
	else if (obLeft)
	{
		if (position.x > idleOb_x)
		{
			body->SetLinearVelocity({ -speed * dt, body->GetLinearVelocity().y });
		}
		else
		{
			obLeft = false;
		}
	}
}

void Ground_Enemies::CheckPlayer()
{
	if (position.x + detectionRange > app->player->GetPosition().x && position.x - detectionRange < app->player->GetPosition().x
		&& position.y + detectionRange > app->player->GetPosition().y && position.y - detectionRange < app->player->GetPosition().y)
	{
		if (state != ENEMY_STATE::HUNT)
		{
			state = ENEMY_STATE::HUNT;
		}
	}
	else
	{
		if (state == ENEMY_STATE::HUNT)
		{
			state = ENEMY_STATE::RETURN;
		}
	}
}

void Ground_Enemies::EnemyHunting(float dt)
{
	PathFinding* path = new PathFinding();

	path->CreatePath({ (int)position.x, 0 }, { (int)app->player->GetPosition().x, 0 });
	int ob_x = path->GetLastPath()->At(path->GetLastPath()->Count() - 1)->x;

	body->SetLinearVelocity({ (ob_x - position.x) * speed * dt, body->GetLinearVelocity().y });

	path_save = path;
}

void Ground_Enemies::EnemyReturning(float dt)
{
	PathFinding* path = new PathFinding();
	
	path->CreatePath({ (int)position.x, 0 }, { (int)origin_x, 0 });
	int ob_x = path->GetLastPath()->At(path->GetLastPath()->Count() - 1)->x;

	body->SetLinearVelocity({ (ob_x - position.x) * speed * dt, body->GetLinearVelocity().y });

	if (position.x + 1.5f > origin_x && position.x - 1.5f < origin_x)
	{
		state = ENEMY_STATE::IDLE;
	}

	if (position.y + 1.5f > origin_y || position.y - 1.5f < origin_y)
	{
		origin_x = position.x;
		origin_y = position.y;
	}

	path_save = path;
}
//
//
//
//
//
// 
// AIR ENEMY
Air_Enemies::Air_Enemies() : Entity()
{
	int pix = 64;

	// walk slime
	floper_walkAnim.PushBack({ pix * 0, pix * 0, pix, pix });
	floper_walkAnim.PushBack({ pix * 1, pix * 0, pix, pix });
	floper_walkAnim.PushBack({ pix * 2, pix * 0, pix, pix });
	floper_walkAnim.PushBack({ pix * 3, pix * 0, pix, pix });
	floper_walkAnim.PushBack({ pix * 4, pix * 0, pix, pix });
	floper_walkAnim.PushBack({ pix * 5, pix * 0, pix, pix });
	floper_walkAnim.speed = 0.02f;

}

// Destructor
Air_Enemies::~Air_Enemies()
{}

// Called each loop iteration
void Air_Enemies::PreUpdate()
{
	if (state != ENEMY_STATE::DEATH)
	{
		position.x = body->GetPosition().x;
		position.y = body->GetPosition().y;
	}
}

// Called each loop iteration
void Air_Enemies::Update(float dt)
{
	currentAnimation->Update();

	// update path
	switch (state)
	{
	case ENEMY_STATE::IDLE:
		if (cd_air_enemy <= 0)
		{
			MoveAirEnemy(dt);
		}
		else
		{
			cd_air_enemy--;
		}

		CheckAirEnemy(dt);
		break;
	case ENEMY_STATE::HUNT:
		EnemyHunting(dt);
		break;
	case ENEMY_STATE::RETURN:
		EnemyReturning(dt);
		break;
	default:
		break;
	}

	if (state != ENEMY_STATE::DEATH)
	{
		CheckPlayer();
	}
}

// Called each loop iteration
void Air_Enemies::Draw()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	if (plan_to_delete)
	{
		app->physics->world->DestroyBody(body);
		plan_to_delete = false;
	}

	if (state != ENEMY_STATE::DEATH)
	{
		app->render->DrawTexture(app->entities->floper_texture, METERS_TO_PIXELS(position.x - (30)), METERS_TO_PIXELS(position.y - (35)), &rect);

		if (state == ENEMY_STATE::HUNT || state == ENEMY_STATE::RETURN)
		{
			if (app->physics->debug)
			{
				app->pathfinding->DrawPath(path_save, position, ENTITY_TYPE::AIR_ENEMY);
			}
		}
	}
}

void Air_Enemies::CreateAirEnemy(float x, float y)
{
	// body
	b2BodyDef e_body;
	e_body.type = b2_dynamicBody;
	e_body.fixedRotation = true;
	e_body.position.Set(PIXELS_TO_METERS(x), PIXELS_TO_METERS(y));
	e_body.gravityScale = 0.0f;

	body = app->physics->world->CreateBody(&e_body);
	body->SetFixedRotation(true);

	b2PolygonShape box;
	box.SetAsBox(PIXELS_TO_METERS(w), PIXELS_TO_METERS(h));

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.friction = 0.0f;
	b2Fixture* bodyFixture = body->CreateFixture(&fixture);
	bodyFixture->SetSensor(false);
	bodyFixture->SetUserData((void*)4); // player collision

	// ground sensor
	box.SetAsBox(PIXELS_TO_METERS(w), PIXELS_TO_METERS(4), b2Vec2(0, PIXELS_TO_METERS(-20)), 0);
	fixture.isSensor = true;
	b2Fixture* sensorFixture = body->CreateFixture(&fixture);
	sensorFixture->SetUserData((void*)9); // ground sensor

	// stats
	origin_x = PIXELS_TO_METERS(x);
	origin_y = PIXELS_TO_METERS(y);
	position.x = PIXELS_TO_METERS(x);
	position.y = PIXELS_TO_METERS(y);
	speed = 0.05f;

	currentAnimation = &floper_walkAnim;

	detectionRange = 5.0f;
	enemy_spoted = false;

	state = ENEMY_STATE::IDLE;
}

void Air_Enemies::ReviveAirEnemy()
{
	// body
	b2BodyDef e_body;
	e_body.type = b2_dynamicBody;
	e_body.fixedRotation = true;
	e_body.position.Set(PIXELS_TO_METERS(position.x), PIXELS_TO_METERS(position.y));
	e_body.gravityScale = 0.0f;

	body = app->physics->world->CreateBody(&e_body);
	body->SetFixedRotation(true);

	b2PolygonShape box;
	box.SetAsBox(PIXELS_TO_METERS(w), PIXELS_TO_METERS(h));

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.friction = 0.0f;
	b2Fixture* bodyFixture = body->CreateFixture(&fixture);
	bodyFixture->SetSensor(false);
	bodyFixture->SetUserData((void*)4); // player collision

	// ground sensor
	box.SetAsBox(PIXELS_TO_METERS(w), PIXELS_TO_METERS(4), b2Vec2(0, PIXELS_TO_METERS(-20)), 0);
	fixture.isSensor = true;
	b2Fixture* sensorFixture = body->CreateFixture(&fixture);
	sensorFixture->SetUserData((void*)9); // ground sensor

	state = ENEMY_STATE::IDLE;
}

void Air_Enemies::MoveAirEnemy(float dt)
{
	int mov = rand() % 4;

	switch (mov)
	{
	case 0: // up
		body->SetLinearVelocity({ 0, -speed * dt });
		break;
	case 1: // down
		body->SetLinearVelocity({ 0, speed * dt });
		break;
	case 2: // left
		body->SetLinearVelocity({ -speed * dt, 0 });
		break;
	case 3: // right
		body->SetLinearVelocity({ speed * dt, 0 });
		break;
	default:
		break;
	}

	cd_air_enemy = 200;
}

void Air_Enemies::CheckAirEnemy(float dt)
{
	int mov = rand() % 3;

	if (position.y + PIXELS_TO_METERS(32 * 5) < origin_y) // up
	{
		body->SetTransform({ body->GetPosition().x, origin_y - PIXELS_TO_METERS(32 * 5) }, 0);

		switch (mov)
		{
		case 0: // down
			body->SetLinearVelocity({ 0, speed * dt });
			break;
		case 1: // left
			body->SetLinearVelocity({ -speed * dt, 0 });
			break;
		case 2: // right
			body->SetLinearVelocity({ speed * dt, 0 });
			break;
		default:
			break;
		}
	}
	else if (position.y - PIXELS_TO_METERS(32 * 5) > origin_y) // down
	{
		body->SetTransform({ body->GetPosition().x, origin_y + PIXELS_TO_METERS(32 * 5) }, 0);

		switch (mov)
		{
		case 0: // up
			body->SetLinearVelocity({ 0, -speed * dt });
			break;
		case 1: // left
			body->SetLinearVelocity({ -speed * dt, 0 });
			break;
		case 2: // right
			body->SetLinearVelocity({ speed * dt, 0 });
			break;
		default:
			break;
		}
	}
	else if (position.x + PIXELS_TO_METERS(32 * 5) < origin_x) // left
	{
		body->SetTransform({ origin_x - PIXELS_TO_METERS(32 * 5), body->GetPosition().y }, 0);

		switch (mov)
		{
		case 0: // up
			body->SetLinearVelocity({ 0, -speed * dt });
			break;
		case 1: // down
			body->SetLinearVelocity({ 0, speed * dt });
			break;
		case 2: // right
			body->SetLinearVelocity({ speed * dt, 0 });
			break;
		default:
			break;
		}
	}
	else if (position.x - PIXELS_TO_METERS(32 * 5) > origin_x) // right
	{
		body->SetTransform({ origin_x + PIXELS_TO_METERS(32 * 5), body->GetPosition().y }, 0);

		switch (mov)
		{
		case 0: // up
			body->SetLinearVelocity({ 0, -speed * dt });
			break;
		case 1: // down
			body->SetLinearVelocity({ 0, speed * dt });
			break;
		case 2: // left
			body->SetLinearVelocity({ -speed * dt, 0 });
			break;
		default:
			break;
		}
	}
}

void Air_Enemies::CheckPlayer()
{
	if (position.x + detectionRange > app->player->GetPosition().x && position.x - detectionRange < app->player->GetPosition().x
		&& position.y + detectionRange > app->player->GetPosition().y && position.y - detectionRange < app->player->GetPosition().y)
	{
		if (state != ENEMY_STATE::HUNT)
		{
			state = ENEMY_STATE::HUNT;
		}
	}
	else
	{
		if (state == ENEMY_STATE::HUNT)
		{
			state = ENEMY_STATE::RETURN;
		}
	}
}

void Air_Enemies::EnemyHunting(float dt)
{
	PathFinding* path = new PathFinding();

	path->CreatePath({ (int)position.x, (int)position.y }, { (int)app->player->GetPosition().x, (int)app->player->GetPosition().y });
	int ob_x = path->GetLastPath()->At(path->GetLastPath()->Count() - 1)->x;
	int ob_y = path->GetLastPath()->At(path->GetLastPath()->Count() - 1)->y;

	body->SetLinearVelocity({ (ob_x - position.x) * speed * dt,  (ob_y - position.y) * speed * dt });

	path_save = path;
}

void Air_Enemies::EnemyReturning(float dt)
{
	PathFinding* path = new PathFinding();

	path->CreatePath({ (int)position.x, (int)position.y }, { (int)origin_x, (int)origin_y });
	int ob_x = path->GetLastPath()->At(path->GetLastPath()->Count() - 1)->x;
	int ob_y = path->GetLastPath()->At(path->GetLastPath()->Count() - 1)->y;

	body->SetLinearVelocity({ (ob_x - position.x) * speed * dt,  (ob_y - position.y) * speed * dt });

	if (position.x + 1.5f > origin_x && position.x - 1.5f < origin_x && position.y + 2.0f > origin_y && position.y - 2.0f < origin_y)
	{
		state = ENEMY_STATE::IDLE;
	}

	path_save = path;
}