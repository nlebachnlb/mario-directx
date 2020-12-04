#include "HudPanel.h"
#include "AnimationDatabase.h"
#include "Game.h"

void HudPanel::Awake()
{
	auto sprManager = Game::GetInstance().GetService<SpriteManager>();
	panel = sprManager->Get("spr-hud-0");
}

void HudPanel::Start()
{
}

void HudPanel::Render()
{
	panel->Draw(32, 594, 0, 0);
}
