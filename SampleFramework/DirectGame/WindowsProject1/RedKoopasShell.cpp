#include "RedKoopasShell.h"
#include "Game.h"
#include "AnimationDatabase.h"

void RedKoopasShell::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Run", animations->Clone("ani-red-koopa-troopa-shell-run"));
	AddAnimation("Idle", animations->Clone("ani-red-koopa-troopa-shell-idle"));
}
