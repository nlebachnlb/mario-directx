#include "Collider2D.h"
#include "Game.h"

Collider2D::Collider2D()
{
    gameObject = nullptr;
    localPosition = VectorZero();
    boxSize = Vector2(1.0f, 1.0f);
    isTrigger = false;
	this->name = "";
	this->pushCoefficient = 0.1f;
}

void Collider2D::Update()
{
    if (gameObject == nullptr || gameObject->IsEnabled() == false) return;
	auto dt = Game::DeltaTime();

	this->dvx = gameObject->GetRigidbody()->GetVelocity().x * dt;
	this->dvy = gameObject->GetRigidbody()->GetVelocity().y * dt;
}

void Collider2D::SweptAABB(RectF movingBox, float dx, float dy, RectF staticBox, float& time, Vector2& direction)
{
	float ml = movingBox.left;
	float mr = movingBox.right;
	float mt = movingBox.top;
	float mb = movingBox.bottom;

	float sl = staticBox.left;
	float sr = staticBox.right;
	float st = staticBox.top;
	float sb = staticBox.bottom;

	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	float t = -1.0f;			// no collision
	float nx, ny;
	nx = ny = 0;

#pragma region Broadphase

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) return;


	if (dx == 0 && dy == 0) return;	
#pragma endregion

#pragma region Sweep
	if (dx > 0)
	{
		dx_entry = sl - mr;
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl - mr;
	}

	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}

	if (dx == 0)
	{
		tx_entry = -99999999999;
		tx_exit = 99999999999;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -99999999999;
		ty_exit = 99999999999;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}

	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	t = t_entry;

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}
#pragma endregion
	// Return result
	direction = Vector2(nx, ny);
	time = t;
}

CollisionEvent* Collider2D::SweptAABBEx(Collider2D* other)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	auto otherBox = other->GetBoundingBox();
	sl = otherBox.left;
	st = otherBox.top;
	sr = otherBox.right;
	sb = otherBox.bottom;

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	auto velocity = other->GetGameObject()->GetRigidbody()->GetVelocity();
	svx = velocity.x;
	svy = velocity.y;

	float sdx = svx * Game::DeltaTime();
	float sdy = svy * Game::DeltaTime();

	float dx = this->dvx - sdx;
	float dy = this->dvy - sdy;

	Vector2 direction;

	SweptAABB(
		GetBoundingBox(),
		dx, dy,
		otherBox,
		t, direction
	);

	CollisionEvent* e = new CollisionEvent(t, direction, other);
	return e;
}

void Collider2D::CalcPotentialCollisions(vector<Collider2D*>* coObjects, vector<CollisionEvent*>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i) == this) continue;

		/*auto bypassTags = gameObject->GetRigidbody()->GetMaterial().bypass;
		auto otherTag = coObjects->at(i)->GetGameObject()->GetTag();
		bool bypass = false;
		for (auto tag : bypassTags)
			bypass = bypass || tag == otherTag;
		if (bypass) continue;*/
		auto otherTag = coObjects->at(i)->GetGameObject()->GetTag();
		auto selfTag = gameObject->GetTag();
		if ((TagUtils::MarioTag(selfTag) && otherTag == ObjectTags::FriendlyProjectiles) || 
			(selfTag == ObjectTags::FriendlyProjectiles && TagUtils::MarioTag(otherTag)))
			continue;

		CollisionEvent* e = SweptAABBEx(coObjects->at(i)); 

		if (e->time > 0 && e->time <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CollisionEvent::Comparator);
}

void Collider2D::FilterCollision(vector<CollisionEvent*>& coEvents, vector<CollisionEvent*>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;
	int minEdge_ix = -1, minEdge_iy = -1;
	float minEdge_tx = 1.0f, minEdge_ty = 1.0f;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		CollisionEvent* c = coEvents[i];

		if (c->time < min_tx && c->collisionDirection.x != 0) 
		{
			min_tx = c->time; 
			nx = c->collisionDirection.x; 
			min_ix = i;
		}

		if (c->time < min_ty && c->collisionDirection.y != 0) 
		{
			min_ty = c->time; 
			ny = c->collisionDirection.y; 
			min_iy = i;
		}
	}

	// WARNING to myself: 
	//   The effector processing for ghost platform still has potential bugs in the future
	//   One of them is a solid object inside a ghostplatform, Mario collides with two
	//     but instead of being blocked, he passes through the solid due to effector of ghost platform
	if (min_ix >= 0)
	{
		coEventsResult.push_back(coEvents[min_ix]);
		auto effector = coEvents[min_ix]->collider->GetGameObject()->GetEffector();
		auto otherBox = coEvents[min_ix]->collider->GetBoundingBox();
		auto selfBox = GetBoundingBox();
		switch (effector)
		{
		case Effector2D::Left:
			if (nx < 0 == false && selfBox.right < otherBox.left == false)
			{
				nx = 0;
				ny = 0;
				min_tx = 1.0f;
				min_ty = 1.0f;
			}
			break;
		case Effector2D::Right:
			if (nx > 0 == false && selfBox.left > otherBox.right == false)
			{
				nx = 0;
				ny = 0;
				min_tx = 1.0f;
				min_ty = 1.0f;
			}
			break;
		case Effector2D::Top:
			nx = 0;
			min_tx = 1.0f;
			if (ny < 0 == false && selfBox.bottom < otherBox.top == false)
			{
				ny = 0;
				min_ty = 1.0f;
			}
			break;
		case Effector2D::Bottom:
			nx = 0;
			min_tx = 1.0f;
			if (ny > 0 == false && selfBox.top > otherBox.bottom == false)
			{
				ny = 0;
				min_ty = 1.0f;
			}
			break;
		}
	}
	if (min_iy >= 0)
	{
		coEventsResult.push_back(coEvents[min_iy]);
		auto effector = coEvents[min_iy]->collider->GetGameObject()->GetEffector();
		auto otherBox = coEvents[min_iy]->collider->GetBoundingBox();
		auto selfBox = GetBoundingBox();
		switch (effector)
		{
		case Effector2D::Left:
			if (nx < 0 == false && selfBox.right < otherBox.left == false)
			{
				nx = 0;
				ny = 0;
				min_tx = 1.0f;
				min_ty = 1.0f;
			}
			break;
		case Effector2D::Right:
			if (nx > 0 == false && selfBox.left > otherBox.right == false)
			{
				nx = 0;
				ny = 0;
				min_tx = 1.0f;
				min_ty = 1.0f;
			}
			break;
		case Effector2D::Top:
			nx = 0;
			min_tx = 1.0f;
			if (ny < 0 == false && selfBox.bottom < otherBox.top == false)
			{
				ny = 0;
				min_ty = 1.0f;
			}
			break;
		case Effector2D::Bottom:
			nx = 0;
			min_tx = 1.0f;
			if (ny > 0 == false && selfBox.top > otherBox.bottom == false)
			{
				ny = 0;
				min_ty = 1.0f;
			}
			break;
		}
	}
}

void Collider2D::PhysicsUpdate(vector<Collider2D*>* coObjects)
{
	if (gameObject == nullptr || gameObject->IsEnabled() == false
		|| gameObject->GetRigidbody()->IsDynamic() == false) return;

	auto dt = Game::DeltaTime();

	auto rigidbody = gameObject->GetRigidbody();
	auto velocity = rigidbody->GetVelocity();
	velocity.y += rigidbody->GetGravity() * dt;
	rigidbody->SetVelocity(&velocity);
	
	vector<CollisionEvent*> coEvents;
	vector<CollisionEvent*> coEventsResult;

	coEvents.clear();
	coEventsResult.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	auto pos = gameObject->GetTransform().Position;

	if (coEvents.size() == 0)
	{
		pos.x += dvx;
		pos.y += dvy;
		gameObject->SetPosition(pos);
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		pos = gameObject->GetTransform().Position;
		
		if (nx != 0 || ny != 0)
		{
			if (isTrigger) gameObject->OnTriggerEnter(this, coEventsResult);
			else gameObject->OnCollisionEnter(this, coEventsResult);
		}

		if (isTrigger == false)
		{
			// DebugOut(L"**ColEv: %f, %f, %f, %f\n", min_tx, min_ty, nx, ny);
			pos.x += min_tx * dvx + nx * pushCoefficient;
			pos.y += min_ty * dvy + ny * pushCoefficient;

			if (ny != 0)
			{
				if (rigidbody->GetGravity() == 0)
				{
					velocity.y = 0;
					dvy = 0;
					rigidbody->SetVelocity(&velocity);
				}
				else
				{
					if (nx == 0)
					{
						// DebugOut(L"DEBUG\n");
						{
							velocity.y = -rigidbody->GetMaterial().bounciness;
							dvy = -rigidbody->GetMaterial().bounciness * Game::DeltaTime();
							rigidbody->SetVelocity(&velocity);
						}
					}
				}
			}

			if (nx != 0)
			{
				velocity.x = 0;
				dvx = 0;
				rigidbody->SetVelocity(&velocity);
			}
		}

		gameObject->SetPosition(pos);
		rigidbody->SetVelocity(&velocity);

		// DebugOut(L"HIT\n");
	}

	for (unsigned i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Collider2D::AttachToEntity(GameObject gameObject)
{
    this->gameObject = gameObject;
}

GameObject Collider2D::GetGameObject()
{
    return gameObject;
}

void Collider2D::SetBoxSize(Vector2 size)
{
    boxSize = size;
}

Vector2 Collider2D::GetBoxSize()
{
    return boxSize;
}

void Collider2D::SetLocalPosition(Vector2 position)
{
    localPosition = position;
}

Vector2 Collider2D::GetLocalPosition()
{
    return localPosition;
}

bool Collider2D::IsEnabled()
{
    return gameObject != nullptr;
}

Vector2 Collider2D::GetWorldPosition()
{
    return gameObject->GetTransform().Position + localPosition;
}

void Collider2D::SetTrigger(bool value)
{
    isTrigger = value;
}

bool Collider2D::IsTrigger()
{
    return isTrigger;
}

void Collider2D::SetName(std::string name)
{
	this->name = name;
}

std::string Collider2D::GetName()
{
	return this->name;
}

RectF Collider2D::GetBoundingBox()
{
	if (gameObject->IsEnabled() == false || enabled == false)
		return RectF{ 0, 0, 0, 0 };

    auto pos = GetWorldPosition();
    RectF RectF;
    RectF.left   = pos.x - boxSize.x * 0.5f;
    RectF.right  = pos.x + boxSize.x * 0.5f;
    RectF.top    = pos.y - boxSize.y * 0.5f;
    RectF.bottom = pos.y + boxSize.y * 0.5f;
	
    return RectF;
}

void Collider2D::SetPushCoefficient(float value)
{
	this->pushCoefficient = value;
}

void Collider2D::Enable()
{
	this->enabled = true;
}

void Collider2D::Disable()
{
	this->enabled = false;
}
