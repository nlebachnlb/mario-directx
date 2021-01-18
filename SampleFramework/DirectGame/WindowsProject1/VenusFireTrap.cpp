#include "VenusFireTrap.h"
#include "VenusFireball.h"
#include "Mathf.h"
#include "Game.h"

void VenusFireTrap::Awake()
{
	Plant::Awake();
	
	poolRegistered = false;
	renderOrder = 13;
	for (int i = 0; i < VENUS_N_POOLED_BULLETS; ++i)
	{
		auto fireball = Instantiate<VenusFireball>();
		fireball->SetPool(&bulletPool);
		bulletPool.Add(fireball);
	}
}

void VenusFireTrap::Start()
{
	Plant::Start();

	waitTime = VENUS_WAIT_TIME;
	speed = VENUS_SPEED;
	hideDistance = VENUS_HIDE_DISTANCE;
	targetLocking = false;
}

void VenusFireTrap::PreRender()
{
	std::string dir = verticalDirection == 1 ? "HeadDown" : "HeadUp";
	switch (movementPhase)
	{
	case 0:
	case 2:
		if (currentState.compare("Reveal" + dir) != 0) SetState("Reveal" + dir);
		break;
	case 1:
	case 3:
		if (currentState.compare("Idle" + dir) != 0) SetState("Idle" + dir);
		break;
	}
	SetScale(Vector2(-direction, 1));
}

void VenusFireTrap::LateUpdate()
{
	AbstractEnemy::LateUpdate();

	if (movementPhase <= 1) UpdateDirection();

	auto dt = Game::DeltaTime() * Game::GetTimeScale();
	if (targetLocking)
	{
		shootTimer += dt;
		if (shootTimer > VENUS_SHOOT_WAIT_TIME)
		{
			shootTimer = 0;
			targetLocking = false;
			auto bullet = bulletPool.Instantiate();
			
			if (bullet != nullptr)
			{
				auto startPos = transform->Position - Vector2(0, GetBoxSize().y * 0.25f);
				bullet->SetPosition(startPos);

				Vector2 directionalVector = Mathf::Normalize(player->GetTransform().Position - startPos);
				
				#pragma region This codeblock enables Venus to shoot absolutely precious to Mario
				/*auto angle = Mathf::Rad2Deg(Mathf::ToAngle(directionalVector));

				if (Mathf::InRange(Mathf::Abs(angle), 45, 135))
				{
					if (Mathf::Abs(angle) < 90) angle = Mathf::Sign(angle) * 45;
					else angle = Mathf::Sign(angle) * 135;
				}*/
				#pragma endregion
				
				float angle = 0;
				auto distance = player->GetTransform().Position - startPos;

				if (Mathf::Abs(distance.x) > 48 * 6 || Mathf::InRange(Mathf::Abs(distance.y), 0, 48 * 2))
					angle = (distance.x > 0 ? 25 : 155) * Mathf::Sign(distance.y);
				else 
					angle = (distance.x > 0 ? 45 : 135) * Mathf::Sign(distance.y);

				angle = Mathf::Deg2Rad(angle);
				directionalVector = Mathf::ToDirectionalVector(angle);
				
				Vector2 velocity = directionalVector * VENUS_BULLET_SPEED;
				bullet->GetRigidbody()->SetVelocity(&velocity);
				/*auto d = VENUS_BULLET_SPEED * Mathf::ToDirectionalVector(Mathf::ToAngle(velocity));
				DebugOut(L"Shoot: %f, %f, %f, %f, %f\n", velocity.x, velocity.y, Mathf::Rad2Deg(Mathf::ToAngle(velocity)), d.x, d.y);*/
			
				if (bullet->GetInGrid())
					bullet->GetCell()->GetContainingGrid()->UpdateObject(bullet);
			}
		}
	}
}

void VenusFireTrap::OnRevealed()
{
	targetLocking = true;
	shootTimer = 0;
}

void VenusFireTrap::OnEnabled()
{
	AbstractEnemy::OnEnabled();

	if (!poolRegistered)
	{
		auto scene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();
		if (scene != nullptr)
		{
			// DebugOut(L"Venus regis\n");
			bulletPool.RegisterPoolToScene(scene);
			poolRegistered = true;
		}
	}
}

ObjectPool& VenusFireTrap::GetBullets()
{
	return this->bulletPool;
}

Vector2 VenusFireTrap::GetBoxSize()
{
	return VENUS_BBOX;
}

void VenusFireTrap::UpdateDirection()
{
	AbstractEnemy::UpdateDirection();
	verticalDirection = player != nullptr ? 
		Mathf::Sign(player->GetTransform().Position.y - (transform->Position.y - 0.25f * GetBoxSize().y)) : 1;
}
