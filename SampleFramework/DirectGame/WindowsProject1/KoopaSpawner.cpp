#include "KoopaSpawner.h"
#include "RedKoopasShell.h"
#include "Game.h"
#include "GreenKoopasShell.h"
#include "GreenKoopa.h"
#include "GreenKoopaParatroopa.h"
#include "RedKoopaParatroopa.h"
#include "BoomerangBrother.h"

void KoopaSpawner::Initialization()
{
	
}

KoopasShell* KoopaSpawner::InstantiateShell(Vector2 position, KoopasShellType shellType, bool reset)
{
	KoopasShell* shell = nullptr;
	ObjectPool* shells;
	if (shellType == KoopasShellType::Red) shells = redShells;
	else shells = greenShells;
	auto scene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();

	if (shells->IsEmpty())
	{
		switch (shellType)
		{
		case KoopasShellType::Green:
			shell = Instantiate<GreenKoopasShell>();
			break;
		case KoopasShellType::Red:
			shell = Instantiate<RedKoopasShell>();
			break;
		}
		scene->AddObject(shell);
		shell->SetPosition(position);
	}
	else
	{
		shell = static_cast<KoopasShell*>(shells->Instantiate());
		shell->SetPosition(position);
		if (scene->GetGrid() != nullptr) scene->GetGrid()->UpdateObject(shell);
	}
	
	if (reset) shell->Start();
	shell->SetPool(shells);

	return shell;
}

AbstractEnemy* KoopaSpawner::Spawn(std::string name, Vector2 position, bool reset, int cellx, int celly)
{
	auto scene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();
	if (scene == nullptr) return nullptr;

	auto pool = GetPool(name);

	if (pool->IsEmpty())
	{
		AbstractEnemy* enm = nullptr;
		if (name.compare("enm-green-koopa") == 0)
			enm = Instantiate<GreenKoopa>();
		else if (name.compare("enm-red-koopa") == 0)
			enm = Instantiate<RedKoopa>();
		else if (name.compare("enm-green-para-koopa") == 0)
			enm = Instantiate<GreenKoopaParatroopa>();
		else if (name.compare("enm-red-para-koopa") == 0)
			enm = Instantiate<RedKoopaParatroopa>();
		else if (name.compare("enm-boomerang-brother") == 0)
			enm = Instantiate<BoomerangBrother>();

		DebugOut(L"Koopa-New spawn\n");

		scene->AddObject(enm);
		enm->SetPosition(position);
		enm->Start();
		enm->OnEnabled();
		enm->SetPool(pool);
		if (cellx != -1 && celly != -1) scene->GetGrid()->Insert(enm, cellx, celly);
		return enm;
	}
	else
	{
		AbstractEnemy* enm = static_cast<AbstractEnemy*>(pool->Instantiate());
		enm->SetPool(pool);
		enm->SetPosition(position);
		enm->Start();
		enm->OnEnabled();
		DebugOut(L"Koopa-Old spawn\n");
		if (scene->GetGrid() != nullptr) scene->GetGrid()->UpdateObject(enm);
		return enm;
	}
}
