#pragma once
#include "GameObject.h"
#include "PlayerController.h"

class IntroMarioController : public CGameObject
{
public:
	void Awake() override;
	void Start() override;
	void Update() override;

private:
	PlayerController* mario, * luigi;
	int timer;
	int mAct, lAct;
};

