#include "PSwitch.h"
#include "Game.h"
#include "AnimationDatabase.h"
#include "Brick.h"
#include "Coin.h"

void PSwitch::Awake()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();

	AddAnimation("Default", animations->Get("ani-switch-block"));
	AddAnimation("Pressed", animations->Get("ani-switch-block-pressed"));

	renderOrder = -1;

	Collider2D* box = new Collider2D();
	box->AttachToEntity(this);
	box->SetBoxSize(PSWITCH_BBOX);
	box->SetTrigger(true);
	this->colliders->push_back(box);
	this->rigidbody->SetDynamic(false);
}

void PSwitch::Start()
{
	SetState("Default");
}

void PSwitch::OnOverlapped(Collider2D* self, Collider2D* other)
{
	if (!TagUtils::MarioTag(other->GetGameObject()->GetTag())) return;
	if (!pressed && other->GetBoundingBox().bottom <= self->GetBoundingBox().top + 12 && other->GetGameObject()->GetRigidbody()->GetVelocity().y > 0)
	{
		pressed = true;
		visualRelativePosition.y = 20;
		self->Disable();

		auto bricks = Game::GetInstance().FindGameObjectsOfType<Brick>();
		auto coins = Game::GetInstance().FindGameObjectsOfType<Coin>();
		auto sceneManager = Game::GetInstance().GetService<SceneManager>();
		auto scene = sceneManager->GetActiveScene();

		for (auto brick : bricks)
		{
			// DebugOut(L"brick\n");
			auto coin = Instantiate<Coin>();
			coin->SetPosition(brick->GetTransform().Position);
			scene->AddObject(coin);
			Destroy(brick);
		}

		for (auto coin : coins)
		{
			// DebugOut(L"brick\n");
			auto brick = Instantiate<Brick>();
			brick->SetPosition(coin->GetTransform().Position);
			scene->AddObject(brick);
			Destroy(coin);
		}
	}
}

void PSwitch::PreRender()
{
	if (pressed && currentState.compare("Pressed") != 0) SetState("Pressed");
}
