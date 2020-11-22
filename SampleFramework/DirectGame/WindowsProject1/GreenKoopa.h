#pragma once
#include "RedKoopa.h"
class GreenKoopa : public RedKoopa
{
public:
	void InitAnimations() override;
	void OnDead(bool oneHit)override;
	void OnCollisionEnter(Collider2D* selfCollider, std::vector<CollisionEvent*> collisions) override;
};

