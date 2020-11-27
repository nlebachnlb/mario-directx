#include "RedVenusFireTrap.h"
#include "AnimationDatabase.h"
#include "Game.h"

void RedVenusFireTrap::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("RevealHeadUp", animations->Get("ani-red-venus-fire-trap-headup"));
	AddAnimation("RevealHeadDown", animations->Get("ani-red-venus-fire-trap-headdown"));
	AddAnimation("IdleHeadUp", animations->Get("ani-red-venus-fire-trap-headup-idle"));
	AddAnimation("IdleHeadDown", animations->Get("ani-red-venus-fire-trap-headdown-idle"));
}
