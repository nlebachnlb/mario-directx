#pragma once
#include "GameObject.h"

const float KOOPA_SPEED = 0.12f;

class MenuKoopa : public CGameObject
{
public:
	void Awake() override;
	void Update() override;
};

