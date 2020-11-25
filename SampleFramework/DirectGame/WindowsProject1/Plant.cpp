#include "Plant.h"
#include "Game.h"
#include "Mathf.h"
#include "EffectPool.h"

void Plant::Start()
{
	state = PlantState::Hidden;
	colliders->at(0)->SetTrigger(true);
	movementPhase = 0;
	timer = 0;
}

void Plant::Movement()
{
	auto dt = Game::DeltaTime() * Game::GetTimeScale();
	if (state == PlantState::Reveal)
	{
		switch (movementPhase)
		{
		case 0:
		{
			visualRelativePosition.y -= speed * dt;
			if (visualRelativePosition.y < -GetBoxSize().y)
			{
				visualRelativePosition.y = -GetBoxSize().y;
				movementPhase = 1;
				timer = 0;
				OnRevealed();
			}
		}
		break;
		case 1:
		{
			timer += dt;
			if (timer > waitTime)
			{
				timer = 0;
				movementPhase = 2;
			}
		}
		break;
		case 2:
		{
			visualRelativePosition.y += speed * dt;
			if (visualRelativePosition.y > 0)
			{
				visualRelativePosition.y = 0;
				movementPhase = 3;
				timer = 0;
				TrackPlayerPosition();
				OnHidden();
			}
		}
		break;
		case 3:
		{
			timer += dt;
			if (timer > waitTime)
			{
				timer = 0;
				movementPhase = 0;
			}
		}
		break;
		}
	}

	colliders->at(0)->SetLocalPosition(visualRelativePosition);
}

void Plant::OnDead(bool oneHit)
{
	if (oneHit)
	{
		auto gmap = Game::GetInstance().GetService<GameMap>();
		auto spawner = gmap->GetSpawnerManager();
		auto fxPool = spawner->GetService<EffectPool>();

		fxPool->CreateFX("fx-smoke-spot", transform.Position);
		dead = true;
		time = -1;
	}
}

void Plant::OnRevealed()
{
}

void Plant::OnHidden()
{
}

void Plant::TrackPlayerPosition()
{
	UpdateDirection();
	if (player == nullptr) return;
	
	auto distance = Mathf::Abs(player->GetTransform().Position.x - transform.Position.x);
	if (distance < hideDistance && state == PlantState::Reveal)
	{
		state = PlantState::Hidden;
		// Reset movement phases
		movementPhase = 0;
		timer = 0;
	}
}
