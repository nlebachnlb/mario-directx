#include "MapTree.h"
#include "Game.h"
#include "AnimationDatabase.h"

void MapTree::Awake()
{
	rigidbody->SetDynamic(false);
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Default", animations->Get("ani-tree"));

	SetState("Default");
}
