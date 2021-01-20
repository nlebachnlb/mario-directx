#pragma once
#include "GameObject.h"
#include "PlayerController.h"
#include "Goomba.h"
#include "GreenKoopasShell.h"
#include "RedKoopasShell.h"
#include "RaccoonLeaf.h"

class IntroMarioController : public CGameObject
{
public:
	void Awake() override;
	void Start() override;
	void Update() override;

	int phase;

protected:
	void LuigiActionScript();
	void MarioActionScript();

private:
	PlayerController* mario, * luigi;
	int mtimer, ltimer, gtimer;
	int mAct, lAct;

	// Minor actors
	RedKoopasShell* redShell;
	GreenKoopasShell* greenShell;
	RaccoonLeaf* leaf;
};

