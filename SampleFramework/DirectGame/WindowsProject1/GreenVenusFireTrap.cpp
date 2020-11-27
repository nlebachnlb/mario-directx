#include "GreenVenusFireTrap.h"
#include "AnimationDatabase.h"
#include "Game.h"

void GreenVenusFireTrap::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("RevealHeadUp", animations->Get("ani-green-venus-fire-trap-headup"));
	AddAnimation("RevealHeadDown", animations->Get("ani-green-venus-fire-trap-headdown"));
	AddAnimation("IdleHeadUp", animations->Get("ani-green-venus-fire-trap-headup-idle"));
	AddAnimation("IdleHeadDown", animations->Get("ani-green-venus-fire-trap-headdown-idle"));
}
