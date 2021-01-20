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
	mtimer = ltimer = gtimer = 0;
	phase = 0;
}

void IntroMarioController::Update()
{
	LuigiActionScript();
	MarioActionScript();

	auto dt = Game::DeltaTime() * Game::GetTimeScale();
	switch (phase)
	{
	case 1:
	{
		auto scene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();

		redShell = Instantiate<RedKoopasShell>(); 
		redShell->SetAlwaysUpdate(true);
		redShell->SetPosition(Vector2(586, -32));
		redShell->noWithdraw = true;
		scene->AddObject(redShell);

		greenShell = Instantiate<GreenKoopasShell>();
		greenShell->SetAlwaysUpdate(true);
		greenShell->SetPosition(Vector2(300, -300));
		greenShell->noWithdraw = true;
		scene->AddObject(greenShell);

		/*leaf = Instantiate<RaccoonLeaf>();
		leaf->SetAlwaysUpdate(true);
		leaf->SetPosition(Vector2(412, 128));
		scene->AddObject(leaf);*/

		phase = 2;
	}
	break;
	case 2:
	{
		gtimer += dt;
		if (gtimer > 2000)
		{
			gtimer = 0;
			phase = 3;
			mAct = 4;
		}
	}
	break;
	}
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
		if (marioState->GetTransform().Position.x <= 412 + 42)
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
	case 4:
	{
		marioState->PressVirtualKeyDown(marioState->marioKeySet.Jump);
		marioState->HoldVirtualKey(marioState->marioKeySet.Jump);
		mAct = 15;
		mtimer = 0;
	}
	break;
	case 15:
	{
		mtimer += dt;
		if (mtimer > 1500)
		{
			mtimer = 0;
			mAct = 5;
		}
	}
	break;
	case 5:
	{
		marioState->ReleaseVirtualKey(marioState->marioKeySet.Jump);
		marioState->HoldVirtualKey(marioState->marioKeySet.Attack);
		marioState->HoldVirtualKey(marioState->marioKeySet.Left);

		if (marioState->GetTransform().Position.x <= 190)
		{
			marioState->ReleaseVirtualKey(marioState->marioKeySet.Left);
			marioState->HoldVirtualKey(marioState->marioKeySet.Right);
			mAct = 6;
		}
	}
	break;
	case 6:
	{
		if (marioState->GetTransform().Position.x >= 300)
		{
			marioState->ReleaseVirtualKey(marioState->marioKeySet.Right);
			if (marioState->GetRigidbody()->GetVelocity().x < 0.2f)
			{
				marioState->ReleaseVirtualKey(marioState->marioKeySet.Attack);
				mAct = 7;
				mtimer = 0;
			}
		}
	}
	break;
	case 7:
	{
		mtimer += dt;
		if (mtimer > 2000)
		{
			mtimer = 0;
			marioState->HoldVirtualKey(marioState->marioKeySet.Attack);
			marioState->HoldVirtualKey(marioState->marioKeySet.Right);
			mAct = 8;
		}
	}
	break;
	case 8:
	{
		if (marioState->GetTransform().Position.x > 824 + 32)
		{
			marioState->ReleaseVirtualKey(marioState->marioKeySet.Attack);
			marioState->ReleaseVirtualKey(marioState->marioKeySet.Right);
			mAct = 9;
		}
	}
	break;
	}
}
