#pragma once
#include "Canvas.h"
#include "HudPanel.h"
#include "Text.h"
#include "GameData.h"
#include "PlayerController.h"

const int TRANSITION_TIME = 500;
const int PSWITCH_TIME = 8000;

class MainCanvas : public Canvas
{
public:
	void Awake() override;
	void Start() override;
	void Update() override;
	void PreRender() override;
	void Render() override;

	void SetGameData(GameData* data);
	void ResetTimer();

	void StartGame();
	void StartTransition();

	bool IsSwitchTime();
	void SwitchCoinBrick(bool freeze = false);
	void StartSwitchTimer();
private:
	HudPanel* hud;
	Texture2D mask;
	GameData* gameData;
	int time, pSwitchTimer;
	float alpha;
	bool gameStarted, timeFreeze;
	int transition;
	PlayerController* player;
};

