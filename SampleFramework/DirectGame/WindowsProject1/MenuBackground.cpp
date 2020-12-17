#include "MenuBackground.h"
#include "Game.h"

void MenuBackground::AddElement(DrawInfo info)
{
	auto spr = Game::GetInstance().GetService<SpriteManager>();

	auto sprite = spr->Get(info.spriteID);
	if (sprite != nullptr)
	{
		elements.push_back(info);
		sprites.push_back(sprite);
	}
}

void MenuBackground::Render(Vector2 translation)
{
	for (int i = 0; i < elements.size(); ++i)
		sprites.at(i)->Draw(
			elements.at(i).position.x, elements.at(i).position.y,
			elements.at(i).pivot.x, elements.at(i).pivot.y);
}
