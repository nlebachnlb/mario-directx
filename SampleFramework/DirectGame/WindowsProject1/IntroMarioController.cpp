#include "IntroMarioController.h"
#include "Game.h"

void IntroMarioController::Awake()
{
	auto scene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();
	Vector2 startPosition(32, 642 - 48);

	luigi = new PlayerController();
	luigi->SetActing(true);
	luigi->Initialize();
	luigi->RegisterToScene(scene);
	luigi->SetPosition(startPosition);
	luigi->InitStates();
	scene->AddObject(luigi);
}

void IntroMarioController::Start()
{
	lAct = 0;
	timer = 0;
}

void IntroMarioController::Update()
{
	auto dt = Game::DeltaTime() * Game::GetTimeScale();
	auto luigiState = luigi->GetMario();
	switch (lAct)
	{
	case 0:
	{
		timer += dt;
		if (timer > 500)
		{
			lAct = 1;
			timer = 0;
		}
	}
	break;
	case 1:
	{
		luigiState->HoldVirtualKey(luigiState->marioKeySet.Right);

		if (luigiState->GetTransform().Position.x >= 206)
		{
			lAct = 2;
			luigiState->PressVirtualKeyDown(luigiState->marioKeySet.Jump);
			luigiState->HoldVirtualKey(luigiState->marioKeySet.Jump);
		}
	}
	break;
	case 2:
	{
		if (luigiState->GetTransform().Position.x >= 412 - 32)
		{
			luigiState->ReleaseVirtualKey(luigiState->marioKeySet.Right);
			luigiState->ReleaseVirtualKey(luigiState->marioKeySet.Jump);
			lAct = 3;
		}
	}
	break;
	}
}
