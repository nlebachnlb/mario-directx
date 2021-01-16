#pragma once
#include "AbstractItem.h"

const float MUSHROOM_SPEED = 0.15f;
const float MUSHROOM_GRAVITY = 0.0024f;
const PhysicMaterial MUSHROOM_PHYSIC_MATERIAL(Vector2(MUSHROOM_SPEED, 0), 0);

class RedMushroom : public AbstractItem
{
public:
	void Awake() override;
	void Start() override;
	void OnSproutCompleted() override;
};

