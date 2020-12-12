#include "PSwitch.h"
#include "Game.h"
#include "AnimationDatabase.h"
#include "Brick.h"
#include "Coin.h"
#include "MainCanvas.h"

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
		auto canvas = static_cast<MainCanvas*>(Canvas::GetCanvas("main"));
		canvas->SwitchCoinBrick(true);
		canvas->StartSwitchTimer();
	}
}

void PSwitch::PreRender()
{
	if (pressed && currentState.compare("Pressed") != 0) SetState("Pressed");
}
