#pragma once
#include "GameObject.h"

const Vector2 RACCOON_ATTACK_BOX(32, 32);

class RaccoonAttackBox : public CGameObject
{
public:
	void Awake() override;
	void Start() override;
	void OnCollisionEnter(Collider2D* selfCollider, std::vector<CollisionEvent*> collisions) override;
	void OnOverlapped(Collider2D* selfCollider, Collider2D* otherCollider);
};

