#pragma once
#include "GameObject.h"

const float PLATFORM_MOVING_SPEED = 0.01f;

class AbstractPlatform : public CGameObject
{
public:
	virtual void Awake() override;
	virtual void OnEnabled() override;
	virtual void Start() override;
	virtual void Update() override;
	virtual void OnOffScreen() override;
	virtual void OnTouch();
	virtual void OnCollisionEnter(Collider2D* selfCollider, vector<CollisionEvent*> collisions) override;

	virtual void InitAnimations() = 0;
	virtual Vector2 GetBoxSize() = 0;

	void SetStartVelocity(Vector2 value);

protected:
	void ApplyGravity();
	GameObject player;
	Vector2 startVelocity;
};

