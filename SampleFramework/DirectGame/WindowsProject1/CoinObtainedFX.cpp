#include "CoinObtainedFX.h"
#include "Game.h"
#include "AnimationDatabase.h"
#include "EffectPool.h"
#include "ScoreFX.h"

void CoinObtainedFX::Awake()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();

	AddAnimation("Default", animations->Get("ani-coin"));

	Collider2D* box = new Collider2D();
	box->SetBoxSize(VectorZero());
	box->AttachToEntity(this);
	box->Disable();
	this->colliders->push_back(box);

	rigidbody->SetDynamic(true);
}

void CoinObtainedFX::Start()
{
	SetState("Default");
	rigidbody->SetVelocity(&Vector2(0, -COIN_BOUNCE));
	rigidbody->SetGravity(COIN_GRAVITY);
	transform.Scale.x = 0.8f;
}

void CoinObtainedFX::Update()
{
	if (transform.Position.y > startPosition.y - 48 && rigidbody->GetVelocity().y > 0)
	{
		auto gmap = Game::GetInstance().GetService<GameMap>();
		auto fxPool = gmap->GetSpawnerManager()->GetService<EffectPool>();
		ScoreFX* fx = static_cast<ScoreFX*>(fxPool->CreateFX("fx-score", transform.Position));
		fx->SetLevel(Score::S100);
		pool->Revoke(this);
	}
}
