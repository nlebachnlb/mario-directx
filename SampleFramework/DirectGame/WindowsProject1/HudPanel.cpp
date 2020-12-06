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

	// Init text
	world	= new Text(); 
	life	= new Text();
	score	= new Text();
	coin	= new Text();
	timer	= new Text();

	// Assign font
	auto font = Game::GetInstance().GetGlobalFont();
	world->SetFont(font);
	life->SetFont(font);
	score->SetFont(font);
	coin->SetFont(font);
	timer->SetFont(font);

	// Power Meter
	powerMeter = new PowerMeter();
	powerMeter->Awake();
}

void HudPanel::Start()
{
	world->SetContent("1");
	life->SetContent("4");
	score->SetContent("0002340");
	coin->SetContent("11");
	timer->SetContent("000");

	// Organize layout
	world->SetPosition(Vector2(32 + 110, config.screenHeight - config.hudOffset + 24));
	life->SetPosition(Vector2(32 + 110 - 4, config.screenHeight - config.hudOffset + 48));
	score->SetPosition(Vector2(32 + 150, config.screenHeight - config.hudOffset + 48));

	coin->SetAlignment(TextAlignment::Right);
	coin->SetPosition(Vector2(32 + 448, config.screenHeight - config.hudOffset + 24));
	timer->SetAlignment(TextAlignment::Right);
	timer->SetPosition(Vector2(32 + 448, config.screenHeight - config.hudOffset + 48));

	powerMeter->SetPosition(Vector2(32 + 150, config.screenHeight - config.hudOffset + 20));
	powerMeter->Start();
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

	world->Render();
	life->Render(); 
	score->Render();
	coin->Render();
	timer->Render();

	powerMeter->Render();
}

void HudPanel::SetWorld(int world)
{
	auto str = to_string(world);
	if (this->world->GetContent().compare(str) == 0) return;
	this->world->SetContent(str);
}

void HudPanel::SetLife(int life)
{
	this->life->SetContent(to_string(life));
}

void HudPanel::SetScore(long score)
{
	std::string format = to_string(score);
	while (format.length() < 7) format = "0" + format;
	this->score->SetContent(format);
}

void HudPanel::SetCoin(int coin)
{
	this->coin->SetContent(to_string(coin));
}

void HudPanel::SetTimer(int time)
{
	std::string format = std::to_string(time);
	while (format.length() < 3) format = "0" + format;
	timer->SetContent(format);
}

void HudPanel::DrawCard(ItemCard& card, int x, int y)
{
	auto visual = cardVisuals[(int)card.item];
	visual->Draw(x, y, 0, 0);
}
