#include "QuestionBlock.h"
#include "AnimationDatabase.h"
#include "Game.h"
#include "EffectPool.h"
#include "RedMushroom.h"
#include "RaccoonLeaf.h"
#include "PSwitch.h"
#include "Green1UPMushroom.h"

void QuestionBlock::OnEnabled()
{
	auto go = Game::GetInstance().FindGameObjectWithTag(ObjectTags::Player, true);
	if (go != nullptr) player = static_cast<PlayerController*>(go);
}

void QuestionBlock::Start()
{
	SetTag(ObjectTags::Block);
	SetState("Sealed");
	bouncingState = 0;
	beingBumped = false;
}

void QuestionBlock::LateUpdate()
{
	if (containedItem.quantity > 0)
	{
		if (timeFreeze)
		{
			if (currentState.compare("Freeze") != 0) SetState("Freeze");
		}
		else if (currentState.compare("Sealed") != 0) SetState("Sealed");
	}
	else if (currentState.compare("Empty") != 0) SetState("Empty");

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
			beingBumped = false;
			visualRelativePosition.y = 0, bouncingState = 0;
			if (containedItem.quantity <= 0) bouncingState = -1;

			switch (containedItem.type)
			{
			case ItemTags::Mushroom:
			{
				auto mushroom = Instantiate<RedMushroom>();
				mushroom->SetPosition(transform->Position - Vector2(0, 49));
				auto scene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();
				scene->AddObject(mushroom);
				mushroom->SproutOut();
			}
			break;
			case ItemTags::Green1UP:
			{
				auto mushroom = Instantiate<Green1UPMushroom>();
				mushroom->SetPosition(transform->Position - Vector2(0, 49));
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

void QuestionBlock::Bounce(GameObject obj)
{
	if (bouncingState == 0)
	{
		beingBumped = true;
		lastTick = GetTickCount(), bouncingState = 1;

		switch (containedItem.type)
		{
		case ItemTags::Coin:
		{
			auto gmap = Game::GetInstance().GetService<GameMap>();
			auto spawner = gmap->GetSpawnerManager();
			spawner->GetService<EffectPool>()->CreateFX("fx-coin-obtained", transform->Position);
			Game::GetInstance().GetData()->ModifyCoin(1, true);
		}
		break;
		case ItemTags::Leaf:
		{
			if (player->GetMario()->GetTag() == ObjectTags::SmallMario)
			{
				containedItem.type = ItemTags::Mushroom;
			}
			else
			{
				auto leaf = Instantiate<RaccoonLeaf>();
				leaf->SetPosition(transform->Position - Vector2(0, 48));
				auto scene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();
				scene->AddObject(leaf);
				leaf->SproutOut();
			}
		}
		break;
		case ItemTags::PSwitch:
		{
			auto pSwitch = Instantiate<PSwitch>();
			pSwitch->SetPosition(transform->Position - Vector2(0, 48));
			auto scene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();
			scene->AddObject(pSwitch);

			auto gmap = Game::GetInstance().GetService<GameMap>();
			auto spawner = gmap->GetSpawnerManager();
			auto fxPool = spawner->GetService<EffectPool>();

			fxPool->CreateFX("fx-smoke-spot", transform->Position - Vector2(0, 48));
		}
		break;
		}

		containedItem.quantity--;
		if (containedItem.quantity <= 0)
		{
			if (containedItem.quantity < 0) containedItem.quantity = 0;
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
	AddAnimation("Freeze", animations->Get("ani-question-block-time-freeze"));
	AddAnimation("Empty", animations->Get("ani-empty-block"));
}
