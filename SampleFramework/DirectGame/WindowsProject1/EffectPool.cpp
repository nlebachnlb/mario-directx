#include "EffectPool.h"
#include "CoinObtainedFX.h"
#include "TransformationFX.h"
#include "GrowUpFX.h"
#include "Game.h"
#include "ShrinkDownFX.h"

void EffectPool::Initialization()
{
}

FXObject* EffectPool::CreateFX(std::string fxName, Vector2 position)
{
	auto scene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();
	if (scene == nullptr) return nullptr;

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
		else if (fxName.compare("fx-mario-transformation") == 0)
			fx = Instantiate<TransformationFX>();
		else if (fxName.compare("fx-mario-grow-up") == 0)
			fx = Instantiate<GrowUpFX>();
		else if (fxName.compare("fx-mario-shrink-down") == 0)
			fx = Instantiate<ShrinkDownFX>();
		
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
