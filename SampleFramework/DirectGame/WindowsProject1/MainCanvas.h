#pragma once
#include "Canvas.h"
#include "HudPanel.h"
#include "Text.h"
#include "GameData.h"
#include "PlayerController.h"

const int TRANSITION_TIME = 500;
const int PSWITCH_TIME = 8000;

const std::string COURSE_CLEAR = "COURSE CLEAR ";
const std::string CARD_REWARD = "YOU GOT A CARD ";

enum class GameState
{
	Ready,
	Waiting,
	Run,
	Finish
};

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
	void FinishGame();
	void StartTransition();

	bool IsSwitchTime();
	void SwitchCoinBrick(bool freeze = false);
	void StartSwitchTimer();
private:
	void GameRun();
	void GameFinish();

	HudPanel* hud;
	Texture2D mask;
	GameData* gameData;
	int time, pSwitchTimer;
	float alpha;
	bool timeFreeze;
	GameState gameState;
	int transition;
	PlayerController* player;

	Text* courseClear, *reward;
	int finishStep;
	int finishTimer;
};

