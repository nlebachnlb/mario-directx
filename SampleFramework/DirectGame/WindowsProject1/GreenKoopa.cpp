#include "GreenKoopa.h"
#include "AnimationDatabase.h"
#include "Game.h"
#include "KoopaSpawner.h"
#include "EffectPool.h"

void GreenKoopa::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Walk", animations->Get("ani-green-koopa-troopa-move"));
	AddAnimation("Die", animations->Get("ani-green-koopa-troopa-shell-idle"));
}

void GreenKoopa::OnDead(bool oneHit)
{
	if (dead) return;

	if (oneHit)
	{
		rigidbody->SetGravity(KOOPA_GRAVITY);
		time = KOOPA_DEAD_TIME * 2;
		colliders->at(0)->Disable();
		transform.Scale.y = -1;
		rigidbody->SetVelocity(&Vector2(-rigidbody->GetVelocity().x, KOOPA_DEFLECTION_ON_SHOT));
		SetState("Idle");
		dead = true;
	}
	else
	{
		auto gameMap = Game::GetInstance().GetService<GameMap>();
		auto koopaSpawner = gameMap->GetSpawnerManager()->GetService<KoopaSpawner>();
		auto delta = Vector2(0, KOOPA_BBOX.y - KOOPAS_SHELL_BBOX.y);
		auto shell = koopaSpawner->InstantiateShell(transform.Position + delta * 0.5f, KoopasShellType::Green);
		
		if (hit)
		{
			shell->OnDead(false);
			auto gmap = Game::GetInstance().GetService<GameMap>();
			auto spawner = gmap->GetSpawnerManager();
			auto fxPool = spawner->GetService<EffectPool>();

			fxPool->CreateFX("fx-hit-star", hitPos);
			hit = false;
		}

		time = -1;
		dead = true;
	}
}

void GreenKoopa::OnCollisionEnter(Collider2D* selfCollider, std::vector<CollisionEvent*> collisions)
{
}
