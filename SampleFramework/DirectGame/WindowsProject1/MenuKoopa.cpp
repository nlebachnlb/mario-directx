#include "MenuKoopa.h"
#include "AnimationDatabase.h"
#include "Game.h"

void MenuKoopa::Awake()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Walk", animations->Get("ani-green-koopa-troopa-move"));
	SetState("Walk");
	alwaysUpdate = true;
	transform.Scale.x = -1;
}

void MenuKoopa::Update()
{
	transform.Position.x += KOOPA_SPEED * Game::DeltaTime() * Game::GetTimeScale();
	auto conf = Game::GetInstance().GetGlobalConfigs();
	if (transform.Position.x > conf.screenWidth + 300)
		transform.Position.x = -192;
}
