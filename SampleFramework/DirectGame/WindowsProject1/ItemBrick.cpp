#include "ItemBrick.h"
#include "AnimationDatabase.h"
#include "Game.h"

void ItemBrick::InitAnimation()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Sealed", animations->Get("ani-brick"));
	AddAnimation("Empty", animations->Get("ani-empty-block"));
}
