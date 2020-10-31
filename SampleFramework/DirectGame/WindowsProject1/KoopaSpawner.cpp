#include "KoopaSpawner.h"
#include "RedKoopasShell.h"
#include "Game.h"

void KoopaSpawner::Initialization()
{
	
}

KoopasShell* KoopaSpawner::InstantiateShell(Vector2 position, bool reset)
{
	RedKoopasShell* shell = nullptr;
	if (shells->IsEmpty())
	{
		shell = Instantiate<RedKoopasShell>();
		auto scene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();
		scene->AddObject(shell);
	}
	else
		shell = static_cast<RedKoopasShell*>(shells->Instantiate());
	
	if (reset) shell->Start();
	shell->SetPosition(position);
	shell->SetPool(shells);

	return shell;
}
