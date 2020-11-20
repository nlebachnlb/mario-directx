#include "BrickDebrisFX.h"
#include "AnimationDatabase.h"
#include "Game.h"

void BrickDebrisFX::Awake()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();

	AddAnimation("Default", animations->Get("ani-brick-debris"));

	Collider2D* box = new Collider2D();
	box->SetBoxSize(VectorZero());
	box->AttachToEntity(this);
	box->Disable();
	this->colliders->push_back(box);

	rigidbody->SetDynamic(true);
	SetScale(Vector2(0.65f, 0.65f));
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

	if (!mainCamera->PointInsideCameraView(transform.Position, 48 * 2))
		pool->Revoke(this);
}
