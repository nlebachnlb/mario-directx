#include "AbstractItem.h"
#include "Game.h"

const Vector2 ITEM_BBOX(48, 47);
const float SPROUT_SPEED = 0.1f;

void AbstractItem::Awake()
{
	Collider2D* collider = new Collider2D();
	collider->SetBoxSize(ITEM_BBOX);
	collider->AttachToEntity(this);
	this->colliders->push_back(collider);
}

void AbstractItem::Start()
{
	sprouting = false;
	visualRelativePosition = Vector2(0, 48);
	colliders->at(0)->Disable();
	rigidbody->SetDynamic(false);
}

void AbstractItem::Update()
{
}

void AbstractItem::PreRender()
{
	if (sprouting)
	{
		visualRelativePosition.y -= SPROUT_SPEED * Game::DeltaTime();
		if (visualRelativePosition.y < 0)
		{
			visualRelativePosition.y = 0;
			sprouting = false;
			colliders->at(0)->Enable();
			rigidbody->SetDynamic(true);
			OnSproutCompleted();
		}
	}
}

void AbstractItem::OnOverlapped(Collider2D* self, Collider2D* other)
{

}

void AbstractItem::OnSproutCompleted()
{
}

void AbstractItem::SproutOut()
{
	sprouting = true;
	colliders->at(0)->Disable();
	rigidbody->SetDynamic(false);
}
