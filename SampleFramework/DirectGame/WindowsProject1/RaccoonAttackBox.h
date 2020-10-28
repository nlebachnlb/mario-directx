#pragma once
#include "GameObject.h"

const Vector2 RACCOON_ATTACK_BOX(32, 48);

class RaccoonAttackBox : public CGameObject
{
public:
	void Awake() override;
	void Start() override;
	void OnCollisionEnter(Collider2D* selfCollider, std::vector<CollisionEvent*> collisions) override;
};

