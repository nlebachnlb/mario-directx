#include "MarioFireball.h"
#include "Game.h"
#include "AnimationDatabase.h"
#include "Mathf.h"
#include "AbstractEnemy.h"
#include "EffectPool.h"

void MarioFireball::Awake()
{
	DebugOut(L"Fireball Awake\n");
	SetTag(ObjectTags::FriendlyProjectiles);

	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Default", animations->Get("ani-fire-ball"));

	Collider2D* box = new Collider2D();
	box->AttachToEntity(this);
	box->SetBoxSize(FIREBALL_BBOX);
	box->SetTrigger(false);
	box->SetPushCoefficient(0.4f);
	this->colliders->push_back(box);

	this->rigidbody->SetDynamic(true);
	auto material = FIREBALL_MATERIAL;

	rigidbody->GetMaterial().AddBypassTag(ObjectTags::FriendlyProjectiles);
	rigidbody->GetMaterial().AddBypassTag(ObjectTags::BigMario);
	rigidbody->GetMaterial().AddBypassTag(ObjectTags::PowerupMario);
	rigidbody->GetMaterial().AddBypassTag(ObjectTags::SmallMario);

	this->rigidbody->SetMaterial(material);
}

void MarioFireball::Start()
{
	SetState("Default");
	rigidbody->SetGravity(FIREBALL_GRAVITY);
	mainCamera = nullptr;
}

void MarioFireball::OnCollisionEnter(Collider2D* selfCollider, std::vector<CollisionEvent*> collisions)
{
	for (auto col : collisions)
	{
		if (col->collisionDirection.x != 0 &&
			(TagUtils::StaticTag(col->collider->GetGameObject()->GetTag())))
		{
			Explode();
		}
		else if (TagUtils::EnemyTag(col->collider->GetGameObject()->GetTag()))
		{
			auto enemy = (AbstractEnemy*)col->collider->GetGameObject();
			if (enemy != nullptr)
				enemy->OnDead(true);
			Explode();
		}
	}
}

void MarioFireball::OnOverlapped(Collider2D* selfCollider, Collider2D* otherCollider)
{
	if (TagUtils::StaticTag(otherCollider->GetGameObject()->GetTag()))
		pool->Revoke(this);

	if (TagUtils::EnemyTag(otherCollider->GetGameObject()->GetTag()))
		static_cast<AbstractEnemy*>(otherCollider->GetGameObject())->OnDead(true), Explode();
}

void MarioFireball::LateUpdate()
{
	if (mainCamera == nullptr)
	{
		auto scene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();
		mainCamera = scene->GetMainCamera();
	}

	if (Mathf::InRange(transform.Position.x, mainCamera->GetPosition().x, mainCamera->GetPosition().x + mainCamera->GetViewportSize().x) == false)
	{
		pool->Revoke(this);
	}
}

void MarioFireball::LinkToPool(ObjectPool* pool)
{
	this->pool = pool;
}

void MarioFireball::Explode()
{
	pool->Revoke(this);

	auto gmap = Game::GetInstance().GetService<GameMap>();
	auto spawner = gmap->GetSpawnerManager();
	auto fxPool = spawner->GetService<EffectPool>();

	fxPool->CreateFX("fx-smoke-spot", transform.Position);
}
