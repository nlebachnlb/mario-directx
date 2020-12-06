#include "HudPanel.h"
#include "AnimationDatabase.h"
#include "Game.h"

void HudPanel::Awake()
{
	auto sprManager = Game::GetInstance().GetService<SpriteManager>();
	panel = sprManager->Get("spr-hud-0");
	config = Game::GetInstance().GetGlobalConfigs();

	cards.push_back(ItemCard{ ItemCardType::Empty });
	cards.push_back(ItemCard{ ItemCardType::Empty });
	cards.push_back(ItemCard{ ItemCardType::Empty });

	cardVisuals[(int)ItemCardType::Empty		] = sprManager->Get("spr-empty-card-0");
	cardVisuals[(int)ItemCardType::Mushroom		] = sprManager->Get("spr-super-mushroom-card-0");
	cardVisuals[(int)ItemCardType::Fireflower	] = sprManager->Get("spr-fire-flower-card-0");
	cardVisuals[(int)ItemCardType::Superstar	] = sprManager->Get("spr-star-man-card-0");
}

void HudPanel::Start()
{
}

void HudPanel::Render()
{
	panel->Draw(0 + 32, config.screenHeight - config.hudOffset, 0, 0);

	for (int i = cards.size() - 1; i >= 0; --i)
	{
		int x = config.screenWidth - cardVisuals[i]->GetSpriteWidth() * (cards.size() - i);
		int y = config.screenHeight - config.hudOffset;
		DrawCard(cards[i], x - 32, y);
	}
}

void HudPanel::DrawCard(ItemCard& card, int x, int y)
{
	auto visual = cardVisuals[(int)card.item];
	visual->Draw(x, y, 0, 0);
}
