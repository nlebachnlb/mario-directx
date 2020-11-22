#include "KoopaSpawner.h"
#include "RedKoopasShell.h"
#include "Game.h"
#include "GreenKoopasShell.h"

void KoopaSpawner::Initialization()
{
	
}

KoopasShell* KoopaSpawner::InstantiateShell(Vector2 position, KoopasShellType shellType, bool reset)
{
	KoopasShell* shell = nullptr;
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
		auto scene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();
		scene->AddObject(shell);
	}
	else
		shell = static_cast<KoopasShell*>(shells->Instantiate());
	
	if (reset) shell->Start();
	shell->SetPosition(position);
	shell->SetPool(shells);

	return shell;
}
