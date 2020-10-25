#pragma once
#include "GameObject.h"

class Collider2D;
struct CollisionEvent;
typedef CollisionEvent* PCollisionEvent;

struct CollisionEvent
{
	Collider2D* collider;
	Vector2 collisionDirection;
	float time;

	CollisionEvent(float time, Vector2 direction, Collider2D* collider)
	{
		this->time = time;
		this->collisionDirection = direction;
		this->collider = collider;
	}

	static bool Comparator(const PCollisionEvent &a, const PCollisionEvent &b)
	{
		return a->time < b->time;
	}
};

class CGameObject;
typedef CGameObject* GameObject;

class Collider2D
{
public:
	Collider2D();
	virtual void Update();

	static void SweptAABB(
		RectF movingBox,		// Moving object's box
		float dx,			// dx = vx * dt
		float dy,			// dy = vy * dt
		RectF staticBox,		// Static object's box
		float& time,		// result: collision time
		Vector2& direction	// result: collision direction
	);

	CollisionEvent* SweptAABBEx(Collider2D* other);
	void CalcPotentialCollisions(vector<Collider2D*>* coObjects, vector<CollisionEvent*>& coEvents);
	void FilterCollision(
		vector<CollisionEvent*>& coEvents,
		vector<CollisionEvent*>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny);
	void PhysicsUpdate(vector<Collider2D*>* coObjects);

	void AttachToEntity(GameObject gameObject);
	GameObject GetGameObject();

	void SetBoxSize(Vector2 size);
	Vector2 GetBoxSize();

	void SetLocalPosition(Vector2 position);
	Vector2 GetLocalPosition();

	bool IsEnabled();
	Vector2 GetWorldPosition();

	void SetTrigger(bool value);
	bool IsTrigger();

	void SetName(std::string name);
	std::string GetName();

	RectF GetBoundingBox();

protected:
	GameObject gameObject;
	Vector2 localPosition;
	Vector2 boxSize;
	bool isTrigger;
	std::string name;

private:
	float dvx, dvy;
};

