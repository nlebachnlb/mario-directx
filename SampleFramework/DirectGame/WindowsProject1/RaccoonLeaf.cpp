#include "RaccoonLeaf.h"
#include "Mathf.h"
#include "Game.h"
#include "AnimationDatabase.h"

void RaccoonLeaf::Awake()
{
	AbstractItem::Awake();
	SetTag(ObjectTags::RaccoonLeaf);
	renderOrder = 3;

	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Default", animations->Get("ani-super-leaf-red"));
}

void RaccoonLeaf::Start()
{
	SetState("Default");

	sprouting = false;
	falling = false;
	rigidbody->SetDynamic(true);
	colliders->at(0)->SetTrigger(true);
	rigidbody->SetGravity(LEAF_GRAVITY);
}

void RaccoonLeaf::Update()
{
	if (sprouting)
	{
		if (Mathf::Abs(transform.Position.y - startPosition.y) <= LEAF_REVEAL_DISTANCE)
		{
			rigidbody->SetVelocity(&Vector2(0, -LEAF_REVEAL_FORCE));
		}
		else
		{
			sprouting = false;
		}
	}

	if (rigidbody->GetVelocity().y > 0 && !falling) falling = true;

	if (falling && rigidbody->GetVelocity().y > LEAF_FALLING_VEL)
		rigidbody->SetVelocity(&Vector2(0, LEAF_FALLING_VEL));
}

void RaccoonLeaf::PreRender()
{
	if (!falling) return;
	lifeTime += Game::DeltaTime();

	auto a = LEAF_AMPLITUDE;
	auto t = LEAF_ROUNDTRIP_TIME;
	auto omega = 2 * Mathf::Pi / (float)t;
	auto phi = Mathf::Pi;
	auto movement = Mathf::Cos(omega * lifeTime + phi);

	prevVisualPos = visualRelativePosition;
	visualRelativePosition.x = a * movement + a;
	colliders->at(0)->SetLocalPosition(Vector2(visualRelativePosition.x, colliders->at(0)->GetLocalPosition().y));
	SetScale(Vector2(-Mathf::Sign(visualRelativePosition.x - prevVisualPos.x), 1));
}

void RaccoonLeaf::SproutOut()
{
	startPosition = transform.Position;
	sprouting = true;
	falling = false;
	lifeTime = 0;
}
