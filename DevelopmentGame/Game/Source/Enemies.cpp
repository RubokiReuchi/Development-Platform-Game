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

// Called before render is available
/*bool Enemies::Awake()
{
	srand(time(NULL));

	return true;
}

// Called before the first frame
bool Enemies::Start()
{
	slime_textureR = app->tex->Load("Assets/textures/SlimeR.png");
	slime_textureL = app->tex->Load("Assets/textures/SlimeL.png");
	floper_texture = app->tex->Load("Assets/textures/Floper.png");

	return true;
}*/

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
		MoveGroundEnemy(enemies.At(i), dt);
		break;
	case ENEMY_STATE::HUNT:
		EnemyHunting(enemies.At(i), dt);
		break;
	case ENEMY_STATE::RETURN:
		EnemyReturning(enemies.At(i), dt);
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
		CheckPlayer(enemies.At(i));
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
			app->render->DrawTexture(slime_textureL, METERS_TO_PIXELS(position.x - (40)), METERS_TO_PIXELS(position.y - (45)), &rect);
		}
		else
		{
			app->render->DrawTexture(slime_textureR, METERS_TO_PIXELS(position.x - (40)), METERS_TO_PIXELS(position.y - (45)), &rect);
		}

		if (state == ENEMY_STATE::HUNT || state == ENEMY_STATE::RETURN)
		{
			if (app->physics->debug)
			{
				app->pathfinding->DrawPath(path_save, enemies.At(i));
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

/*void Enemies::CreateAirEnemy(float x, float y)
{
	Enemy* new_enemy = new Enemy();

	// body
	b2BodyDef e_body;
	e_body.type = b2_dynamicBody;
	e_body.fixedRotation = true;
	e_body.position.Set(PIXELS_TO_METERS(x), PIXELS_TO_METERS(y));
	e_body.gravityScale = 0.0f;

	new_enemy->body = app->physics->world->CreateBody(&e_body);
	new_enemy->body->SetFixedRotation(true);

	b2PolygonShape box;
	box.SetAsBox(PIXELS_TO_METERS(new_enemy->w), PIXELS_TO_METERS(new_enemy->h));

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.friction = 0.0f;
	b2Fixture* bodyFixture = new_enemy->body->CreateFixture(&fixture);
	bodyFixture->SetSensor(false);
	bodyFixture->SetUserData((void*)4); // player collision

	// ground sensor
	box.SetAsBox(PIXELS_TO_METERS((new_enemy->w)), PIXELS_TO_METERS(4), b2Vec2(0, PIXELS_TO_METERS(-20)), 0);
	fixture.isSensor = true;
	b2Fixture* sensorFixture = new_enemy->body->CreateFixture(&fixture);
	sensorFixture->SetUserData((void*)9); // ground sensor

	// stats
	new_enemy->origin_x = PIXELS_TO_METERS(x);
	new_enemy->origin_y = PIXELS_TO_METERS(y);
	new_enemy->x = PIXELS_TO_METERS(x);
	new_enemy->y = PIXELS_TO_METERS(y);
	new_enemy->speed = 0.05f;

	new_enemy->type = ENEMY_TYPE::AIR;

	new_enemy->currentAnimation = &floper_walkAnim;

	new_enemy->lookLeft = true;

	new_enemy->detectionRange = 5.0f;
	new_enemy->enemy_spoted = false;

	new_enemy->state = ENEMY_STATE::IDLE;
	
	enemies.Insert(*new_enemy, enemies.Count());
}*/

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

/*void Enemies::ReviveAirEnemy(Enemy* enemy)
{
	// body
	b2BodyDef e_body;
	e_body.type = b2_dynamicBody;
	e_body.fixedRotation = true;
	e_body.position.Set(PIXELS_TO_METERS(enemy->x), PIXELS_TO_METERS(enemy->y));
	e_body.gravityScale = 0.0f;

	enemy->body = app->physics->world->CreateBody(&e_body);
	enemy->body->SetFixedRotation(true);

	b2PolygonShape box;
	box.SetAsBox(PIXELS_TO_METERS(enemy->w), PIXELS_TO_METERS(enemy->h));

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.friction = 0.0f;
	b2Fixture* bodyFixture = enemy->body->CreateFixture(&fixture);
	bodyFixture->SetSensor(false);
	bodyFixture->SetUserData((void*)4); // player collision

	// ground sensor
	box.SetAsBox(PIXELS_TO_METERS((enemy->w)), PIXELS_TO_METERS(4), b2Vec2(0, PIXELS_TO_METERS(-20)), 0);
	fixture.isSensor = true;
	b2Fixture* sensorFixture = enemy->body->CreateFixture(&fixture);
	sensorFixture->SetUserData((void*)9); // ground sensor

	enemy->state = ENEMY_STATE::IDLE;
}*/

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

/*void Enemies::MoveAirEnemy(Enemy* enemy, float dt)
{
	int mov = rand() % 4;

	switch (mov)
	{
	case 0: // up
		enemy->body->SetLinearVelocity({ 0, -enemy->speed * dt });
		break;
	case 1: // down
		enemy->body->SetLinearVelocity({ 0, enemy->speed * dt });
		break;
	case 2: // left
		enemy->body->SetLinearVelocity({ -enemy->speed * dt, 0 });
		break;
	case 3: // right
		enemy->body->SetLinearVelocity({ enemy->speed * dt, 0 });
		break;
	default:
		break;
	}

	enemy->cd_air_enemy = 200;
}*/

/*void Enemies::CheckAirEnemy(Enemy* enemy, float dt)
{
	int mov = rand() % 3;
	
	if (enemy->y + PIXELS_TO_METERS(32 * 5) < enemy->origin_y) // up
	{
		enemy->body->SetTransform({ enemy->body->GetPosition().x, enemy->origin_y - PIXELS_TO_METERS(32 * 5) }, 0);

		switch (mov)
		{
		case 0: // down
			enemy->body->SetLinearVelocity({ 0, enemy->speed * dt });
			break;
		case 1: // left
			enemy->body->SetLinearVelocity({ -enemy->speed * dt, 0 });
			break;
		case 2: // right
			enemy->body->SetLinearVelocity({ enemy->speed * dt, 0 });
			break;
		default:
			break;
		}
	}
	else if (enemy->y - PIXELS_TO_METERS(32 * 5) > enemy->origin_y) // down
	{
		enemy->body->SetTransform({ enemy->body->GetPosition().x, enemy->origin_y + PIXELS_TO_METERS(32 * 5) }, 0);

		switch (mov)
		{
		case 0: // up
			enemy->body->SetLinearVelocity({ 0, -enemy->speed * dt });
			break;
		case 1: // left
			enemy->body->SetLinearVelocity({ -enemy->speed * dt, 0 });
			break;
		case 2: // right
			enemy->body->SetLinearVelocity({ enemy->speed * dt, 0 });
			break;
		default:
			break;
		}
	}
	else if (enemy->x + PIXELS_TO_METERS(32 * 5) < enemy->origin_x) // left
	{
		enemy->body->SetTransform({ enemy->origin_x - PIXELS_TO_METERS(32 * 5), enemy->body->GetPosition().y }, 0);

		switch (mov)
		{
		case 0: // up
			enemy->body->SetLinearVelocity({ 0, -enemy->speed * dt });
			break;
		case 1: // down
			enemy->body->SetLinearVelocity({ 0, enemy->speed * dt });
			break;
		case 2: // right
			enemy->body->SetLinearVelocity({ enemy->speed * dt, 0 });
			break;
		default:
			break;
		}
	}
	else if (enemy->x - PIXELS_TO_METERS(32 * 5) > enemy->origin_x) // right
	{
		enemy->body->SetTransform({ enemy->origin_x + PIXELS_TO_METERS(32 * 5), enemy->body->GetPosition().y }, 0);

		switch (mov)
		{
		case 0: // up
			enemy->body->SetLinearVelocity({ 0, -enemy->speed * dt });
			break;
		case 1: // down
			enemy->body->SetLinearVelocity({ 0, enemy->speed * dt });
			break;
		case 2: // left
			enemy->body->SetLinearVelocity({ -enemy->speed * dt, 0 });
			break;
		default:
			break;
		}
	}
}*/

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
	
/*else if (enemy->type == ENEMY_TYPE::AIR)
	{
		path->CreatePath({ (int)enemy->x, (int)enemy->y }, { (int)app->player->GetPosition().x, (int)app->player->GetPosition().y });
		int ob_x = path->GetLastPath()->At(path->GetLastPath()->Count() - 1)->x;
		int ob_y = path->GetLastPath()->At(path->GetLastPath()->Count() - 1)->y;

		LOG("x = %d, y = %d", ob_x - enemy->x, ob_y - enemy->y);

		enemy->body->SetLinearVelocity({ (ob_x - enemy->x) * enemy->speed* dt,  (ob_y - enemy->y) * enemy->speed * dt });
	}*/

	path_save = path;
}

void Ground_Enemies::EnemyReturning(float dt)
{
	PathFinding* path = new PathFinding();
	
	if (enemy->type == ENEMY_TYPE::GROUND)
	{
		path->CreatePath({ (int)enemy->x, 0 }, { (int)enemy->origin_x, 0 });
		int ob_x = path->GetLastPath()->At(path->GetLastPath()->Count() - 1)->x;
		
		enemy->body->SetLinearVelocity({ (ob_x - enemy->x) * enemy->speed * dt, enemy->body->GetLinearVelocity().y });

		if (enemy->x + 1.5f > enemy->origin_x && enemy->x - 1.5f < enemy->origin_x)
		{
			enemy->state = ENEMY_STATE::IDLE;
		}

		if (enemy->y + 1.5f > enemy->origin_y || enemy->y - 1.5f < enemy->origin_y)
		{
			enemy->origin_x = enemy->x;
			enemy->origin_y = enemy->y;
		}
	}
	else if (enemy->type == ENEMY_TYPE::AIR)
	{
		path->CreatePath({ (int)enemy->x, (int)enemy->y }, { (int)enemy->origin_x, (int)enemy->origin_y });
		int ob_x = path->GetLastPath()->At(path->GetLastPath()->Count() - 1)->x;
		int ob_y = path->GetLastPath()->At(path->GetLastPath()->Count() - 1)->y;

		enemy->body->SetLinearVelocity({ (ob_x - enemy->x) * enemy->speed * dt,  (ob_y - enemy->y) * enemy->speed * dt });

		if (enemy->x + 1.5f > enemy->origin_x && enemy->x - 1.5f < enemy->origin_x && enemy->y + 2.0f > enemy->origin_y && enemy->y - 2.0f < enemy->origin_y)
		{
			enemy->state = ENEMY_STATE::IDLE;
		}
	}

	enemy->path_save = path;
}