#include "QuestionBlock.h"
#include "AnimationDatabase.h"
#include "Game.h"
#include "EffectPool.h"

void QuestionBlock::Start()
{
	SetTag(ObjectTags::QuestBlock);
	SetState("Sealed");
	bouncingState = 0;
}

void QuestionBlock::LateUpdate()
{
	switch (bouncingState)
	{
	case 1:
	{
		visualRelativePosition.y -= BOUNCE_VEL * Game::DeltaTime();
		if (GetTickCount() - lastTick > BOUNCE_TIME)
			lastTick = GetTickCount(), bouncingState = 2;
	}
		break;
	case 2:
	{
		visualRelativePosition.y += BOUNCE_VEL * Game::DeltaTime();
		if (GetTickCount() - lastTick > BOUNCE_TIME)
			visualRelativePosition.y = 0, bouncingState = 0;
	}
		break;
	}
}

void QuestionBlock::Bounce()
{
	if (bouncingState == 0)
	{
		lastTick = GetTickCount(), bouncingState = 1;
		auto gmap = Game::GetInstance().GetService<GameMap>();
		auto spawner = gmap->GetSpawnerManager();
		spawner->GetService<EffectPool>()->CreateFX("fx-coin-obtained", transform.Position);
	}
}

void QuestionBlock::InitAnimation()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Sealed", animations->Get("ani-question-block"));
	AddAnimation("Empty", animations->Get("ani-empty-block"));
}
