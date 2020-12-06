#pragma once
#include "UIElement.h"
#include "HudUtils.h"
#include "Sprite.h"
#include "GlobalConfigs.h"
#include <vector>

class HudPanel : public UIElement
{
public:
	void Awake() override;
	void Start() override;
	void Render() override;

private:
	void DrawCard(ItemCard& card, int x, int y);
	Sprite panel;
	GlobalConfigs config;
	std::vector<ItemCard> cards;
	Sprite cardVisuals[4];
};