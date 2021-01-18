#include "RewardFX.h"
#include "Game.h"
#include "AnimationDatabase.h"

void RewardFX::Awake()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Mushroom", animations->Get("ani-goal-mushroom"));
	AddAnimation("Flower", animations->Get("ani-goal-flower"));
	AddAnimation("Star", animations->Get("ani-goal-star"));
}

void RewardFX::Start()
{
	SetState("Mushroom");
}

void RewardFX::LateUpdate()
{
	transform->Position.y -= REWARD_SPEED * Game::DeltaTime() * Game::GetTimeScale();
}

void RewardFX::OnOffScreen()
{
	pool->Revoke(this);
}

void RewardFX::SetReward(int reward)
{
	this->reward = reward;
	std::string items[3] = { "Mushroom", "Flower", "Star" };
	SetState(items[reward]);
}
