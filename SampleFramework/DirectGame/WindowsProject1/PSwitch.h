#pragma once
#include "GameObject.h"

const Vector2 PSWITCH_BBOX(24, 24);

class PSwitch : public CGameObject
{
public:
	void Awake() override;
	void Start() override;
	void OnOverlapped(Collider2D* self, Collider2D* other) override;
	void PreRender() override;

protected:
	bool pressed;
};

