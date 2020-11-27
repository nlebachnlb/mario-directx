#pragma once
#include "GameObject.h"
#include "ObjectPool.h"

const Vector2 VENUS_FIREBALL_BBOX(8, 8);

class VenusFireball : public CGameObject
{
public:
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnOffScreen() override;
	void SetPool(ObjectPool* pool);

protected:
	ObjectPool* pool;
};

