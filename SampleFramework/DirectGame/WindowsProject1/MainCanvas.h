#pragma once
#include "Canvas.h"
#include "HudPanel.h"
#include "Text.h"
#include "GameData.h"
#include "PlayerController.h"
#include "WorldDialog.h"

const int TRANSITION_TIME = 500;
const int PSWITCH_TIME = 8000;

const std::string COURSE_CLEAR = "COURSE CLEAR ";
const std::string CARD_REWARD = "YOU GOT A CARD ";

enum class GameState
{
	Unload,
	WorldStart,
	Menu,
	Ready,
	Waiting,
	Die,
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
	void OnSceneUnload() override;

	void SetGameData(GameData* data);
	void ResetTimer();

	void StartGame();
	void GetGameReady();
	void LoseGame();
	void FinishGame(int card);
	void StartTransition();
	void StartMenu();
	void CloseMenu();
	void WorldIntro();

	bool IsSwitchTime();
	void SwitchCoinBrick(bool freeze = false);
	void StartSwitchTimer();

	bool IsDialogOpening();
	void OpenWorldDialog();
	void CloseWorldDialog();

	void SetTargetScene(std::string id);
private:
	void GameReady();
	void GameRun();
	void GameFinish();
	void GameLose();
	void GameMenu();
	void GameWorldStart();

	HudPanel* hud;
	Texture2D mask;
	GameData* gameData;
	int time, pSwitchTimer;
	float alpha;
	bool timeFreeze;
	GameState gameState;

	int transition, transTimer;
	Vector2 wipePosition;
	std::string targetSceneID;

	PlayerController* player;

	#pragma region HUD
	Text* courseClear, *reward;
	int finishStep, finishTimer;
	int card;
	Sprite cardVisuals[3];
	#pragma endregion

	#pragma region Dialogs
	WorldDialog* worldDialog;
	bool dialogOpening;
	#pragma endregion
};

