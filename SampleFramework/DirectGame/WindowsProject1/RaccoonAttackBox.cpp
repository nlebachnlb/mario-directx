#include "RaccoonAttackBox.h"
#include "AbstractEnemy.h"
#include "Game.h"
#include "EffectPool.h"
#include "KoopasShell.h"
#include "Mathf.h"

void RaccoonAttackBox::Awake()
{
	SetTag(ObjectTags::MarioAttack);

	Collider2D* box = new Collider2D();
	box->AttachToEntity(this);
	box->SetBoxSize(RACCOON_ATTACK_BOX);
	box->SetTrigger(true);
	box->SetPushCoefficient(0.4f);
	this->colliders->push_back(box);

	this->rigidbody->SetDynamic(true);
	renderOrder = -1;
}

void RaccoonAttackBox::Start()
{
}

void RaccoonAttackBox::OnCollisionEnter(Collider2D* selfCollider, std::vector<CollisionEvent*> collisions)
{

}

void RaccoonAttackBox::OnOverlapped(Collider2D* selfCollider, Collider2D* otherCollider)
{
	auto otherTag = otherCollider->GetGameObject()->GetTag();
	if (TagUtils::EnemyTag(otherTag))
	{
		auto gmap = Game::GetInstance().GetService<GameMap>();
		auto spawner = gmap->GetSpawnerManager();
		auto fxPool = spawner->GetService<EffectPool>();

		fxPool->CreateFX("fx-hit-star", transform.Position);

		switch (otherTag)
		{
		case ObjectTags::KoopasShell:
		{
			SetActive(false);
			selfCollider->Disable();
			auto shell = static_cast<KoopasShell*>(otherCollider->GetGameObject());
			shell->SetFacing(Mathf::Sign(shell->GetTransform().Position.x - transform.Position.x));
			shell->OnDead(false);
		}
		break;
		}
	}
}
