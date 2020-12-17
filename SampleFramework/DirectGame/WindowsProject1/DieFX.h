#pragma once
#include "MarioFX.h"

const float BOUNCE_FORCE = 0.8f;

class DieFX : public MarioFX
{
public:
	void Awake() override;
	void Start() override;
	void Update() override;

private:
	int timer;
	int fxState;
	Vector2 pos;
};

