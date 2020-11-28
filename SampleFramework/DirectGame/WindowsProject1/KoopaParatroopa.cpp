#include "KoopaParatroopa.h"
#include "Game.h"
#include "KoopaSpawner.h"

void KoopaParatroopa::OnDead(bool oneHit)
{
	if (dead) return;

	if (oneHit)
	{
		rigidbody->SetGravity(KOOPA_PARATROOPA_GRAVITY);
		time = KOOPA_PARATROOPA_DEAD_TIME * 2;
		colliders->at(0)->Disable();
		transform.Scale.y = -1;
		rigidbody->SetVelocity(&Vector2(-rigidbody->GetVelocity().x, KOOPA_PARATROOPA_DEFLECTION_ON_SHOT));
		SetState("Die");
		dead = true;
	}
	else
	{
		auto gameMap = Game::GetInstance().GetService<GameMap>();
		auto koopaSpawner = gameMap->GetSpawnerManager()->GetService<KoopaSpawner>();
		std::string color;
		if (troopa == Troopa::Green) color = "green"; else color = "red";
		auto name = "enm-" + color + "-koopa";
		auto koopa = koopaSpawner->Spawn(name, transform.Position);
		rigidbody->SetVelocity(&Vector2(rigidbody->GetVelocity().x, 0));
		rigidbody->PassDataTo(koopa->GetRigidbody());

		time = -1;
		dead = true;
	}
}

void KoopaParatroopa::OnEnabled()
{
	AbstractEnemy::OnEnabled();
	rigidbody->SetVelocity(&Vector2(direction * KOOPA_PARATROOPA_MOVING_SPEED, 0));
}

Vector2 KoopaParatroopa::GetBoxSize()
{
    return KOOPA_PARATROOPA_BBOX;
}
