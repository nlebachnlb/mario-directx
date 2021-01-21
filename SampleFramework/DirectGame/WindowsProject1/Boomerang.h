#pragma once
#include "GameObject.h"
#include "ObjectPool.h"

const Vector2 BOOMERANG_BBOX(16, 16);
const Vector2 BOOMERANG_ACCE(-0.006f, 0.0036f);
const Vector2 BOOMERANG_TURN_BACK_VEL(0.48f, 0);
const Vector2 BOOMERANG_THROWN_VEL(0.48f, -0.23f);

class Boomerang : public CGameObject
{
public:
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnOffScreen() override;
	void SetPool(ObjectPool* pool);
	void OnOverlapped(Collider2D* self, Collider2D* other) override;
	
	GameObject owner;
	int facing;

protected:
	ObjectPool* pool;
	int phase;
};

