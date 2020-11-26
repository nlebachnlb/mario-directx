#include "PlantSpawner.h"
#include "Game.h"
#include "PiranhaPlant.h"

AbstractEnemy* PlantSpawner::Spawn(std::string name, Vector2 position, bool reset)
{
	auto scene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();
	if (scene == nullptr) return nullptr;

	auto pool = GetPool(name);

	if (pool->IsEmpty())
	{
		AbstractEnemy* enm = nullptr;
		if (name.compare("enm-green-piranha-plant") == 0)
			enm = Instantiate<PiranhaPlant>();

		DebugOut(L"Plant-New spawn\n");

		scene->AddObject(enm);
		enm->SetPosition(position);
		enm->Start();
		enm->SetPool(pool);
		return enm;
	}
	else
	{
		AbstractEnemy* enm = static_cast<AbstractEnemy*>(pool->Instantiate());
		enm->SetPool(pool);
		enm->SetPosition(position);
		enm->Start();
		DebugOut(L"Plant-Old spawn\n");
		return enm;
	}
}
