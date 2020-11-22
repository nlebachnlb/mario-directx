#include "GreenKoopasShell.h"
#include "Game.h"
#include "AnimationDatabase.h"

void GreenKoopasShell::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Run", animations->Get("ani-green-koopa-troopa-shell-run"));
	AddAnimation("Idle", animations->Get("ani-green-koopa-troopa-shell-idle"));
}
