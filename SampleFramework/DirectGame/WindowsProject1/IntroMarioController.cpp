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

	startPosition = Vector2(824 - 32, 642 - 48);
	mario = new PlayerController();
	mario->SetActing(true);
	mario->Initialize();
	mario->RegisterToScene(scene);
	mario->SetPosition(startPosition);
	mario->InitStates();
	mario->GetMario()->HoldVirtualKey(mario->GetMario()->marioKeySet.Left);
	mario->GetMario()->ReleaseVirtualKey(mario->GetMario()->marioKeySet.Left);
	scene->AddObject(mario);
}

void IntroMarioController::Start()
{
	lAct = 0;
	mAct = 0;
	mtimer = ltimer = 0;
}

void IntroMarioController::Update()
{
	LuigiActionScript();
	MarioActionScript();
}

void IntroMarioController::LuigiActionScript()
{
	auto dt = Game::DeltaTime() * Game::GetTimeScale();
	auto luigiState = luigi->GetMario();
	switch (lAct)
	{
	case 0:
	{
		ltimer += dt;
		if (ltimer > 500)
		{
			lAct = 1;
			ltimer = 0;
		}
	}
	break;
	case 1:
	{
		luigiState->HoldVirtualKey(luigiState->marioKeySet.Right);

		if (luigiState->GetTransform().Position.x >= 206 - 16)
		{
			lAct = 2;
			luigiState->PressVirtualKeyDown(luigiState->marioKeySet.Jump);
			luigiState->HoldVirtualKey(luigiState->marioKeySet.Jump);
		}
	}
	break;
	case 2:
	{
		if (luigiState->GetTransform().Position.x >= 412)
		{
			//luigiState->ReleaseVirtualKey(luigiState->marioKeySet.Right);
			//luigiState->ReleaseVirtualKey(luigiState->marioKeySet.Jump);
			luigiState->Jump(MARIO_JUMP_FORCE, true);
			lAct = 3;
		}
	}
	break;
	case 3:
	{
		if (luigiState->GetTransform().Position.x >= 824 + 32)
		{
			luigiState->ReleaseVirtualKey(luigiState->marioKeySet.Right);
			luigiState->ReleaseVirtualKey(luigiState->marioKeySet.Jump);
			lAct = 4;
		}
	}
	break;
	}
}

void IntroMarioController::MarioActionScript()
{
	auto dt = Game::DeltaTime() * Game::GetTimeScale();
	auto marioState = mario->GetMario();
	switch (mAct)
	{
	case 0:
	{
		mtimer += dt;
		if (mtimer > 400)
		{
			mAct = 1;
			mtimer = 0;
		}
	}
	break;
	case 1:
	{
		marioState->HoldVirtualKey(marioState->marioKeySet.Left);
		if (marioState->GetTransform().Position.x <= 412 + 32)
		{
			marioState->ReleaseVirtualKey(marioState->marioKeySet.Left);
			marioState->HoldVirtualKey(marioState->marioKeySet.Crouch);
			mAct = 2;
			mtimer = 0;
		}
	}
	break;
	case 2:
	{
		mtimer += dt;
		if (mtimer > 800)
		{
			mtimer = 0;
			mAct = 3;
			marioState->ReleaseVirtualKey(marioState->marioKeySet.Crouch);
		}
	}
	break;
	}
}
