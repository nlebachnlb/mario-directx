#include "RedParagoomba.h"
#include "AnimationDatabase.h"
#include "Game.h"

void RedParagoomba::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Idle", animations->Get("ani-red-goomba-idle"));
	AddAnimation("Walk", animations->Get("ani-red-goomba-walk"));
	AddAnimation("Die", animations->Get("ani-red-goomba-die"));
}
