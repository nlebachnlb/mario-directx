#include "MarioFireball.h"
#include "Game.h"
#include "AnimationDatabase.h"
#include "Mathf.h"

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

	//material.AddBypassTag(ObjectTags::FriendlyProjectiles);
	////material.AddBypassTag(ObjectTags::HostileProjectiles);
	//material.AddBypassTag(ObjectTags::PowerupMario);
	////material.AddBypassTag(ObjectTags::BigMario);
	////material.AddBypassTag(ObjectTags::SmallMario);

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
		if (//col->collisionDirection.y < 0 && 
			col->collisionDirection.x != 0 &&
			(col->collider->GetGameObject()->GetTag() == ObjectTags::Solid ||
			col->collider->GetGameObject()->GetTag() == ObjectTags::GhostPlatform))
		{
			pool->Revoke(this);
		}
	}
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
