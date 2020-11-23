#include "RaccoonAttackBox.h"
#include "AbstractEnemy.h"

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
}

void RaccoonAttackBox::Start()
{
}

void RaccoonAttackBox::OnCollisionEnter(Collider2D* selfCollider, std::vector<CollisionEvent*> collisions)
{
	/*for (auto col : collisions)
	{
		if (TagUtils::EnemyTag(col->collider->GetGameObject()->GetTag()))
		{
			DebugOut(L"Enemy collision\n");
			auto enemy = (AbstractEnemy*)col->collider->GetGameObject();
			if (enemy != nullptr)
				enemy->OnDead(true);
		}
	}*/
}

void RaccoonAttackBox::OnOverlapped(Collider2D* selfCollider, Collider2D* otherCollider)
{
	//DebugOut(L"Attack box overlap\n");
	//if (TagUtils::EnemyTag(otherCollider->GetGameObject()->GetTag()))
	//{
	//	DebugOut(L"Enemy collision\n");
	//	/*auto enemy = (AbstractEnemy*)otherCollider->GetGameObject();
	//	if (enemy != nullptr)
	//		enemy->OnDead(true);*/
	//}
}
