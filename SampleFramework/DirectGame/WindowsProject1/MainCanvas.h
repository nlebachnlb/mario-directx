#pragma once
#include "Canvas.h"
#include "HudPanel.h"
#include "Text.h"
#include "GameData.h"
#include "PlayerController.h"

const int TRANSITION_TIME = 500;

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
private:
	HudPanel* hud;
	Texture2D mask;
	GameData* gameData;
	int time;
	float alpha;
	bool gameStarted;
	int transition;
	PlayerController* player;
};

