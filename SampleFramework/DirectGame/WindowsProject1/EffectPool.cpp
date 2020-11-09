#include "EffectPool.h"
#include "CoinObtainedFX.h"
#include "Game.h"

void EffectPool::Initialization()
{
}

FXObject* EffectPool::CreateFX(std::string fxName, Vector2 position)
{
	auto scene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();

	ObjectPool* pool;
	if (pools.find(fxName) != pools.end())
		pool = pools.at(fxName);
	else
		pool = new ObjectPool(), pools.insert(std::make_pair(fxName, pool));

	if (pool->IsEmpty())
	{
		FXObject* fx = nullptr;
		if (fxName.compare("fx-coin-obtained") == 0)
			fx = Instantiate<CoinObtainedFX>();
		
		DebugOut(L"New spawn\n");

		scene->AddObject(fx);
		fx->SetPosition(position);
		fx->SetStartPosition(position);
		fx->Start();
		fx->SetPool(pool);
		return fx;
	}
	else
	{
		FXObject* fx = static_cast<FXObject*>(pool->Instantiate());
		fx->SetPool(pool);
		fx->SetPosition(position);
		fx->SetStartPosition(position);
		fx->Start();
		DebugOut(L"Old spawn\n");
		return fx;
	}
}
