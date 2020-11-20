#pragma once
#include "FXObject.h"

const float COIN_GRAVITY = 0.0024f;
const float COIN_BOUNCE = 0.9f;

class CoinObtainedFX : public FXObject
{
public:
	void Awake() override;
	void Start() override;
	void Update() override;
};

