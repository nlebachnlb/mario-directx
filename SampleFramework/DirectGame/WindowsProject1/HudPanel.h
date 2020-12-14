#pragma once
#include "UIElement.h"
#include "HudUtils.h"
#include "Sprite.h"
#include "GlobalConfigs.h"
#include <vector>
#include "Text.h"
#include "PowerMeter.h"

class HudPanel : public UIElement
{
public:
	void Awake() override;
	void Start() override;
	void PreRender() override;
	void Render() override;

	void SetWorld(int world);
	void SetLife(int life);
	void SetScore(long score);
	void SetCoin(int coin);
	void SetTimer(int time);
	void SetPowerMeter(int level);

	void StartBlinkingLastCard();
	void StopBlinkingLastCard();

private:
	void DrawCard(ItemCard& card, int x, int y);
	Sprite panel;
	GlobalConfigs config;
	std::vector<ItemCard> cards;
	Sprite cardVisuals[4];

	Text *world, *life, *score, *coin, *timer;
	PowerMeter* powerMeter;

	int blinkTimer;
	bool blinkLastCard;
	ItemCardType lastCard;
};