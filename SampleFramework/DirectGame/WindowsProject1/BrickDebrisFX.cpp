#include "BrickDebrisFX.h"
#include "AnimationDatabase.h"
#include "Game.h"
#include "PhysicConstants.h"
#include "Mathf.h"

void BrickDebrisFX::Awake()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();

	AddAnimation("Default", animations->Get("ani-brick-debris"));

	rigidbody->SetDynamic(false);
	SetScale(Vector2(0.70f, 0.70f));

	renderOrder = 10;
}

void BrickDebrisFX::Start()
{
	SetState("Default");
	rigidbody->SetGravity(DEFAULT_GRAVITY);
}

void BrickDebrisFX::Update()
{
	if (mainCamera == nullptr)
	{
		auto scene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();
		mainCamera = scene->GetMainCamera();
	}

	if (!mainCamera->PointInsideCameraView(transform->Position, 48))
		pool->Revoke(this);

	auto dt = Game::DeltaTime() * Game::GetTimeScale();
	transform->Position = transform->Position + rigidbody->GetVelocity() * dt;

	auto vel = rigidbody->GetVelocity();
	vel.y += rigidbody->GetGravity() * dt;
	vel.y = Mathf::Min(vel.y, DEFAULT_FALL_LIMIT_VEL);
	rigidbody->SetVelocity(&vel);
}
