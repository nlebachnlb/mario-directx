#pragma once
#include "GameObject.h"
#include <unordered_map>

class AbstractEnemy : public CGameObject
{
public:
	virtual void Awake() override;
	virtual void Update() override;

	virtual Vector2 GetBoxSize() = 0;
	virtual void Movement() = 0;
	virtual void InitAnimations() = 0;
	virtual void OnDead() = 0;
};

