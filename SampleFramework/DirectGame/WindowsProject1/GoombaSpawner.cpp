#include "GoombaSpawner.h"
#include "Goomba.h"
#include "RedGoomba.h"
#include "RedParagoomba.h"
#include "Game.h"

AbstractEnemy* GoombaSpawner::Spawn(std::string name, Vector2 position, bool reset, int cellx, int celly)
{
	auto scene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();
	if (scene == nullptr) return nullptr;

	auto pool = GetPool(name);

	if (pool->IsEmpty())
	{
		AbstractEnemy* enm = nullptr;
		if (name.compare("enm-tan-goomba") == 0)
			enm = Instantiate<Goomba>();
		else if (name.compare("enm-red-goomba") == 0)
			enm = Instantiate<RedGoomba>();
		else if (name.compare("enm-red-para-goomba") == 0)
			enm = Instantiate<RedParagoomba>();

		DebugOut(L"Goomba-New spawn\n");

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
		DebugOut(L"Goomba-Old spawn\n");
		if (scene->GetGrid() != nullptr) scene->GetGrid()->UpdateObject(enm);
		return enm;
	}
}
