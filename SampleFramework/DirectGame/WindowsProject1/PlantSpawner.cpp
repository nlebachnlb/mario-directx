#include "PlantSpawner.h"
#include "Game.h"
#include "PiranhaPlant.h"
#include "RedVenusFireTrap.h"
#include "GreenVenusFireTrap.h"

AbstractEnemy* PlantSpawner::Spawn(std::string name, Vector2 position, bool reset, int cellx, int celly)
{
	auto scene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();
	if (scene == nullptr) return nullptr;

	auto pool = GetPool(name);

	if (pool->IsEmpty())
	{
		AbstractEnemy* enm = nullptr;
		if (name.compare("enm-green-piranha-plant") == 0)
			enm = Instantiate<PiranhaPlant>();
		else if (name.compare("enm-red-venus-fire-trap") == 0)
			enm = Instantiate<RedVenusFireTrap>();
		else if (name.compare("enm-green-venus-fire-trap") == 0)
			enm = Instantiate<GreenVenusFireTrap>();

		DebugOut(L"Plant-New spawn\n");

		scene->AddObject(enm);
		enm->SetPosition(position);
		enm->Start();
		enm->OnEnabled();
		enm->SetPool(pool);
		// scene->GetGrid()->Insert(enm);
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
		if (scene->GetGrid() != nullptr) scene->GetGrid()->UpdateObject(enm);
		DebugOut(L"Plant-Old spawn\n");
		return enm;
	}
}
