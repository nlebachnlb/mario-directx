#pragma once
#include "AbstractItem.h"

const float GREEN_MUSHROOM_SPEED = 0.15f;
const float GREEN_MUSHROOM_GRAVITY = 0.0024f;
const PhysicMaterial GREEN_MUSHROOM_PHYSIC_MATERIAL(Vector2(GREEN_MUSHROOM_SPEED, 0), 0);

class Green1UPMushroom : public AbstractItem
{
public:
	void Awake() override;
	void Start() override;
	void OnSproutCompleted() override;
};

