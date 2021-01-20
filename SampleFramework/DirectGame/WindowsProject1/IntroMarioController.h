#pragma once
#include "GameObject.h"
#include "PlayerController.h"

class IntroMarioController : public CGameObject
{
public:
	void Awake() override;
	void Start() override;
	void Update() override;

protected:
	void LuigiActionScript();
	void MarioActionScript();

private:
	PlayerController* mario, * luigi;
	int mtimer, ltimer;
	int mAct, lAct;
};

