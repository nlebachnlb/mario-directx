#pragma once
#include "GameObject.h"

const Vector2 FIREBALL_BBOX(8, 8);

class MarioFireball : public CGameObject
{
public:
	void Awake() override;
	void Start() override;
};

