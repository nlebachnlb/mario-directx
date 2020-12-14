#pragma once
#include "FXObject.h"

const float REWARD_SPEED = 0.25f;

class RewardFX : public FXObject
{
public:
	void Awake() override;
	void Start() override;
	void LateUpdate() override;
	void OnOffScreen() override;

	void SetReward(int reward);

private:
	int reward;
};

