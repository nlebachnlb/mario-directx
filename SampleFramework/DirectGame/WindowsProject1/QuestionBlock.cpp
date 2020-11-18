#include "QuestionBlock.h"
#include "AnimationDatabase.h"
#include "Game.h"
#include "EffectPool.h"
#include "RedMushroom.h"

void QuestionBlock::Start()
{
	SetTag(ObjectTags::QuestBlock);
	SetState("Sealed");
	bouncingState = 0;
}

void QuestionBlock::PreRender()
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
		{
			visualRelativePosition.y = 0, bouncingState = 0;
			if (containedItem.quantity <= 0) bouncingState = -1;

			switch (containedItem.type)
			{
			case ItemTags::Mushroom:
			{
				auto mushroom = Instantiate<RedMushroom>();
				mushroom->SetPosition(transform.Position - Vector2(0, 49));
				auto scene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();
				scene->AddObject(mushroom);
				mushroom->SproutOut();
			}
			break;
			}
		}
	}
		break;
	}
}

void QuestionBlock::Bounce()
{
	if (bouncingState == 0)
	{
		lastTick = GetTickCount(), bouncingState = 1;

		switch (containedItem.type)
		{
		case ItemTags::Coin:
		{
			auto gmap = Game::GetInstance().GetService<GameMap>();
			auto spawner = gmap->GetSpawnerManager();
			spawner->GetService<EffectPool>()->CreateFX("fx-coin-obtained", transform.Position);
		}
		break;
		}

		containedItem.quantity--;
		if (containedItem.quantity <= 0)
		{
			if (containedItem.quantity < 0) containedItem.quantity = 0;
			if (currentState.compare("Empty") != 0) SetState("Empty");
		}
	}
}

void QuestionBlock::SetItem(ItemInfo item)
{
	this->containedItem = item;
}

void QuestionBlock::InitAnimation()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("Sealed", animations->Get("ani-question-block"));
	AddAnimation("Empty", animations->Get("ani-empty-block"));
}
