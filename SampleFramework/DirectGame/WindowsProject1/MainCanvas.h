#pragma once
#include "Canvas.h"
#include "HudPanel.h"
#include "Text.h"
#include "GameData.h"

class MainCanvas : public Canvas
{
public:
	void Awake() override;
	void Start() override;
	void PreRender() override;
	void Render() override;

	void SetGameData(GameData* data);
	void ResetTimer();

	void StartGame();
private:
	HudPanel* hud;
	Texture2D mask;
	GameData* gameData;
	int time;
	bool gameStarted;
};

