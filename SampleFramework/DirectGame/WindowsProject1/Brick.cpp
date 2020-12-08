#include "Brick.h"
#include "AnimationDatabase.h"
#include "Game.h"
#include "EffectPool.h"

void Brick::Start()
{
	SetState("Default");
	bouncingState = 0;
}

void Brick::InitAnimation()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Default", animations->Get("ani-brick"));
}

void Brick::PreRender()
{
	switch (bouncingState)
	{
	case 1:
	{
		visualRelativePosition.y -= BOUNCE_VEL * Game::DeltaTime();
		if (GetTickCount() - lastTick > BOUNCE_TIME)
			lastTick = GetTickCount(), bouncingState = 2;
	}
	break;
	case 2:
	{
		visualRelativePosition.y += BOUNCE_VEL * Game::DeltaTime();
		if (GetTickCount() - lastTick > BOUNCE_TIME)
			visualRelativePosition.y = 0, bouncingState = 0;
	}
	break;
	}
}

void Brick::Bounce(GameObject obj)
{
	auto oTag = obj->GetTag();

	if (oTag == ObjectTags::SmallMario)
	{
		if (bouncingState == 0) lastTick = GetTickCount(), bouncingState = 1;
	}
	else
		Explode();
}

void Brick::OnOverlapped(Collider2D* self, Collider2D* other)
{
	if (other->GetGameObject()->GetTag() == ObjectTags::MarioAttack)
	{
		Bounce(other->GetGameObject());
		other->GetGameObject()->SetActive(false);
		other->GetGameObject()->GetColliders()->at(0)->Disable();
	}
}

void Brick::Explode()
{
	auto gmap = Game::GetInstance().GetService<GameMap>();
	auto spawner = gmap->GetSpawnerManager();
	auto fxPool = spawner->GetService<EffectPool>();

	const float velx[4] = { +0.15f, +0.20f, -0.20f, -0.15f };
	const float vely[4] = { -0.30f, -0.60f, -0.60f, -0.30f };

	for (int i = 0; i < 4; ++i)
	{
		auto debris = fxPool->CreateFX("fx-brick-debris", transform.Position);
		debris->GetRigidbody()->SetVelocity(new Vector2(velx[i] * 1.25f, vely[i] * 1.25f));
	}

	Destroy(this);
}
