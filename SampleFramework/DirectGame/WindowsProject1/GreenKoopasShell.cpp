#include "GreenKoopasShell.h"
#include "Game.h"
#include "AnimationDatabase.h"
#include "KoopaSpawner.h"
#include "GreenKoopa.h"

void GreenKoopasShell::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Run", animations->Get("ani-green-koopa-troopa-shell-run"));
	AddAnimation("Idle", animations->Get("ani-green-koopa-troopa-shell-idle"));
	AddAnimation("Withdraw", animations->Get("ani-green-koopa-troopa-crouch"));
}

void GreenKoopasShell::OnWithdraw()
{
	auto gameMap = Game::GetInstance().GetService<GameMap>();
	auto koopaSpawner = gameMap->GetSpawnerManager()->GetService<KoopaSpawner>();
	auto delta = Vector2(0, KOOPA_BBOX.y - KOOPAS_SHELL_BBOX.y);
	auto obj = koopaSpawner->Spawn("enm-green-koopa", transform->Position - delta * 0.5f);
}
