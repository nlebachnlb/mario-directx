#include "LevelReward.h"
#include "Game.h"
#include "CMario.h"
#include "EffectPool.h"
#include "RewardFX.h"
#include "MainCanvas.h"

void LevelReward::Awake()
{
	auto sprMng = Game::GetInstance().GetService<SpriteManager>();
	items[0] = sprMng->Get("spr-super-mushroom-0");
	items[1] = sprMng->Get("spr-fire-flower-0");
	items[2] = sprMng->Get("spr-star-man-0");

	Collider2D* box = new Collider2D();
	box->SetBoxSize(REWARD_BBOX);
	box->AttachToEntity(this);
	box->SetTrigger(true);
	this->colliders->push_back(box);
	this->rigidbody->SetDynamic(false);
}

void LevelReward::Start()
{
	reward = 0;
	timer = 0;
}

void LevelReward::PreRender()
{
	auto dt = Game::DeltaTime() * Game::GetTimeScale();
	timer += dt;
	if (timer > REWARD_CHANGE_TIME)
	{
		timer = 0;
		reward = (reward + 1 > 2 ? 0 : reward + 1);
	}
}

void LevelReward::Render(Vector2 translation)
{
	items[reward]->Draw(transform->Position.x + translation.x, transform->Position.y + translation.y);
}

void LevelReward::OnOverlapped(Collider2D* self, Collider2D* other)
{
	if (TagUtils::MarioTag(other->GetGameObject()->GetTag()))
	{
		auto mario = static_cast<CMario*>(other->GetGameObject());
		mario->FinishLevel();
		mario->GetRigidbody()->SetVelocity(&VectorZero());

		auto gmap = Game::GetInstance().GetService<GameMap>();
		auto fxPool = gmap->GetSpawnerManager()->GetService<EffectPool>();
		RewardFX* fx = static_cast<RewardFX*>(fxPool->CreateFX("fx-reward", transform->Position));
		fx->SetReward(reward);

		auto canvas = static_cast<MainCanvas*>(Canvas::GetCanvas("main"));
		canvas->FinishGame(reward);

		Destroy(this);
	}
}
