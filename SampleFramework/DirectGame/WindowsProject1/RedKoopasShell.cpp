#include "RedKoopasShell.h"
#include "Game.h"
#include "AnimationDatabase.h"
#include "KoopaSpawner.h"
#include "RedKoopa.h"

void RedKoopasShell::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Run", animations->Get("ani-red-koopa-troopa-shell-run"));
	AddAnimation("Idle", animations->Get("ani-red-koopa-troopa-shell-idle"));
	AddAnimation("Withdraw", animations->Get("ani-red-koopa-troopa-crouch"));
}

void RedKoopasShell::OnWithdraw()
{
	auto gameMap = Game::GetInstance().GetService<GameMap>();
	auto koopaSpawner = gameMap->GetSpawnerManager()->GetService<KoopaSpawner>();
	auto delta = Vector2(0, KOOPA_BBOX.y - KOOPAS_SHELL_BBOX.y);
	auto obj = koopaSpawner->Spawn("enm-red-koopa", transform->Position - delta * 0.5f);
}
