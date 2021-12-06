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

Enemies::Enemies() : Module()
{
	name.Create("enemies");
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
	
}

// Destructor
Enemies::~Enemies()
{}

// Called before render is available
bool Enemies::Awake()
{
	srand(time(NULL));

	return true;
}

// Called before the first frame
bool Enemies::Start()
{
	textureR = app->tex->Load("Assets/textures/Player.png");
	textureL = app->tex->Load("Assets/textures/PlayerInv.png");

	return true;
}

// Called each loop iteration
bool Enemies::PreUpdate()
{
	for (size_t i = 0; i < enemies.Count(); i++)
	{
		if (enemies.At(i)->state != ENEMY_STATE::DEATH)
		{
			enemies.At(i)->x = enemies.At(i)->body->GetPosition().x;
			enemies.At(i)->y = enemies.At(i)->body->GetPosition().y;

			if (enemies.At(i)->obLeft)
			{
				enemies.At(i)->idleOb_x = enemies.At(i)->origin_x - PIXELS_TO_METERS(32 * 5);
			}
			else
			{
				enemies.At(i)->idleOb_x = enemies.At(i)->origin_x + PIXELS_TO_METERS(32 * 5);
			}
		}
	}

	return true;
}

// Called each loop iteration
bool Enemies::Update(float dt)
{
	for (size_t i = 0; i < enemies.Count(); i++)
	{
		Enemy* en = enemies.At(i);

		en->currentAnimation->Update();

		// update path
		if (en->type == ENEMY_TYPE::GROUND)
		{
			if (en->state == ENEMY_STATE::IDLE)
			{
				MoveGroundEnemy(enemies.At(i), dt);
				
			}
			else if (en->state == ENEMY_STATE::HUNT)
			{
				EnemyHunting(enemies.At(i), dt);
			}
			else if (en->state == ENEMY_STATE::RETURN)
			{
				EnemyReturning(enemies.At(i), dt);
			}
		}
		else if (en->type == ENEMY_TYPE::AIR)
		{
			if (en->state == ENEMY_STATE::IDLE)
			{
				if (en->cd_air_enemy <= 0)
				{
					MoveAirEnemy(enemies.At(i), dt);
				}
				else
				{
					en->cd_air_enemy--;
				}

				CheckAirEnemy(enemies.At(i), dt);
			}
			else if (en->state == ENEMY_STATE::HUNT)
			{
				EnemyHunting(enemies.At(i), dt);
			}
			else if (en->state == ENEMY_STATE::RETURN)
			{
				EnemyReturning(enemies.At(i), dt);
			}
		}

		if (en->state != ENEMY_STATE::DEATH)
		{
			CheckPlayer(enemies.At(i));
		}
	}
	
	return true;
}

// Called each loop iteration
bool Enemies::PostUpdate()
{
	for (size_t i = 0; i < enemies.Count(); i++)
	{
		SDL_Rect rect = enemies.At(i)->currentAnimation->GetCurrentFrame();

		if (!app->scene->GetStartScreenState())
		{
			for (size_t i = 0; i < enemies.Count(); i++)
			{
				if (enemies.At(i)->plan_to_delete)
				{
					app->physics->world->DestroyBody(enemies.At(i)->body);
					enemies.At(i)->plan_to_delete = false;
				}
				
				if (enemies.At(i)->state != ENEMY_STATE::DEATH)
				{
					if (enemies.At(i)->lookLeft)
					{
						app->render->DrawTexture(textureL, METERS_TO_PIXELS(enemies.At(i)->x), METERS_TO_PIXELS(enemies.At(i)->y), &rect);
					}
					else
					{
						app->render->DrawTexture(textureR, METERS_TO_PIXELS(enemies.At(i)->x), METERS_TO_PIXELS(enemies.At(i)->y), &rect);
					}
				}
			}
		}
	}
	
	return true;
}

// Called before quitting
bool Enemies::CleanUp()
{

	return true;
}

bool Enemies::LoadState(pugi::xml_node& data)
{
	for (size_t i = 0; i < data.attribute("value").as_int(); i++)
	{
		std::string p = "position";
		std::string s = std::to_string(i);
		std::string t = p + s;
		const char* c = t.c_str();

		if (data.child(c).attribute("state").as_int() == 0)
		{
			if (enemies.At(i)->state == ENEMY_STATE::DEATH)
			{
				if (enemies.At(i)->type == ENEMY_TYPE::GROUND)
				{
					ReviveGroundEnemy(enemies.At(i));
				}
				else if (enemies.At(i)->type == ENEMY_TYPE::AIR)
				{
					ReviveAirEnemy(enemies.At(i));
				}
			}
			
			enemies.At(i)->x = data.child(c).attribute("x").as_int();
			enemies.At(i)->y = data.child(c).attribute("y").as_int();

			enemies.At(i)->body->SetTransform({ enemies.At(i)->x + PIXELS_TO_METERS(enemies.At(i)->w), enemies.At(i)->y }, enemies.At(i)->body->GetAngle());
			enemies.At(i)->body->ApplyForceToCenter({ 0, 200 }, true);

			enemies.At(i)->currentAnimation = &idleAnimR;
		}
		else
		{
			enemies.At(i)->state = ENEMY_STATE::DEATH;
		}
	}

	return true;
}

bool Enemies::SaveState(pugi::xml_node& data)
{
	for (size_t i = 0; i < enemies.Count(); i++)
	{
		std::string p = "position";
		std::string s = std::to_string(i);
		std::string t = p + s;
		const char* c = t.c_str();

		data.child(c).attribute("x").set_value(enemies.At(i)->x);
		data.child(c).attribute("y").set_value(enemies.At(i)->y);

		if (enemies.At(i)->state != ENEMY_STATE::DEATH)
		{
			data.child(c).attribute("state").set_value("0");
		}
		else
		{
			data.child(c).attribute("state").set_value("1");
		}
	}

	data.attribute("value").set_value(enemies.Count());

	return true;
}

void Enemies::CreateGroundEnemy(float x, float y)
{
	Enemy* new_enemy = new Enemy();
	
	// body
	b2BodyDef e_body;
	e_body.type = b2_dynamicBody;
	e_body.fixedRotation = true;
	e_body.position.Set(PIXELS_TO_METERS(x), PIXELS_TO_METERS(y));

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
	bodyFixture->SetUserData((void*)4);

	// ground sensor
	box.SetAsBox(PIXELS_TO_METERS((new_enemy->w)), PIXELS_TO_METERS(4), b2Vec2(0, PIXELS_TO_METERS(-20)), 0);
	fixture.isSensor = true;
	b2Fixture* sensorFixture = new_enemy->body->CreateFixture(&fixture);
	sensorFixture->SetUserData((void*)9); // hit sensor

	// stats
	new_enemy->origin_x = PIXELS_TO_METERS(x);
	new_enemy->origin_y = PIXELS_TO_METERS(y);
	new_enemy->x = PIXELS_TO_METERS(x);
	new_enemy->y = PIXELS_TO_METERS(y);
	new_enemy->speed = 0.05f;

	new_enemy->type = ENEMY_TYPE::GROUND;

	new_enemy->currentAnimation = &idleAnimR;

	new_enemy->lookLeft = true;

	new_enemy->detectionRange = 5.0f;
	new_enemy->enemy_spoted = false;

	new_enemy->state = ENEMY_STATE::IDLE;
	new_enemy->obLeft = false;

	enemies.Insert(*new_enemy, enemies.Count());
}

void Enemies::CreateAirEnemy(float x, float y)
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

	new_enemy->currentAnimation = &idleAnimR;

	new_enemy->lookLeft = true;

	new_enemy->detectionRange = 5.0f;
	new_enemy->enemy_spoted = false;

	new_enemy->state = ENEMY_STATE::IDLE;
	
	enemies.Insert(*new_enemy, enemies.Count());
}

void Enemies::ReviveGroundEnemy(Enemy* enemy)
{
	// body
	b2BodyDef e_body;
	e_body.type = b2_dynamicBody;
	e_body.fixedRotation = true;
	e_body.position.Set(PIXELS_TO_METERS(enemy->x), PIXELS_TO_METERS(enemy->y));

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
}

void Enemies::ReviveAirEnemy(Enemy* enemy)
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
}

void Enemies::MoveGroundEnemy(Enemy* enemy, float dt)
{
	if (!enemy->obLeft)
	{
		if (enemy->x < enemy->idleOb_x)
		{
			enemy->body->SetLinearVelocity({ enemy->speed * dt, enemy->body->GetLinearVelocity().y });
		}
		else
		{
			enemy->obLeft = true;
		}

	}
	else if (enemy->obLeft)
	{
		if (enemy->x > enemy->idleOb_x)
		{
			enemy->body->SetLinearVelocity({ -enemy->speed * dt, enemy->body->GetLinearVelocity().y });
		}
		else
		{
			enemy->obLeft = false;
		}
	}
}

void Enemies::MoveAirEnemy(Enemy* enemy, float dt)
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
}

void Enemies::CheckAirEnemy(Enemy* enemy, float dt)
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
}

void Enemies::KillEnemy(float x, float y)
{
	for (size_t i = 0; i < enemies.Count(); i++)
	{
		if (x + 1.5f > enemies.At(i)->x && x - 1.5f < enemies.At(i)->x && y + 2.0f > enemies.At(i)->y && y - 2.0f < enemies.At(i)->y)
		{
			enemies.At(i)->state = ENEMY_STATE::DEATH;
			enemies.At(i)->plan_to_delete = true;
			app->player->player_body->ApplyForceToCenter({ 0, -23.5f * app->GetDT() }, true);
			
			break;
		}
	}
}

void Enemies::CheckPlayer(Enemy* enemy)
{
	if (enemy->x + enemy->detectionRange > app->player->GetPosition().x && enemy->x - enemy->detectionRange < app->player->GetPosition().x
		&& enemy->y + enemy->detectionRange > app->player->GetPosition().y && enemy->y - enemy->detectionRange < app->player->GetPosition().y)
	{
		if (enemy->state != ENEMY_STATE::HUNT)
		{
			enemy->state = ENEMY_STATE::HUNT;
		}
	}
	else
	{
		if (enemy->state == ENEMY_STATE::HUNT)
		{
			enemy->state = ENEMY_STATE::RETURN;
		}
	}
}

void Enemies::EnemyHunting(Enemy* enemy, float dt)
{
	PathFinding* path = new PathFinding();
	;
	if (enemy->type == ENEMY_TYPE::GROUND)
	{
		path->CreatePath({ (int)enemy->x, 0 }, { (int)app->player->GetPosition().x, 0 });
		int ob_x = path->GetLastPath()->At(path->GetLastPath()->Count() - 1)->x;

		enemy->body->SetLinearVelocity({ (ob_x - enemy->x) * enemy->speed * dt, enemy->body->GetLinearVelocity().y });
	}
	else if(enemy->type == ENEMY_TYPE::AIR)
	{
		path->CreatePath({ (int)enemy->x, (int)enemy->y }, { (int)app->player->GetPosition().x, (int)app->player->GetPosition().y });
		int ob_x = path->GetLastPath()->At(path->GetLastPath()->Count() - 1)->x;
		int ob_y = path->GetLastPath()->At(path->GetLastPath()->Count() - 1)->y;

		LOG("x = %d, y = %d", ob_x - enemy->x, ob_y - enemy->y);

		enemy->body->SetLinearVelocity({ (ob_x - enemy->x) * enemy->speed* dt,  (ob_y - enemy->y) * enemy->speed * dt });
	}
	
}

void Enemies::EnemyReturning(Enemy* enemy, float dt)
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

}