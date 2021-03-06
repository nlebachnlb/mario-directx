#include "EffectPool.h"
#include "CoinObtainedFX.h"
#include "TransformationFX.h"
#include "GrowUpFX.h"
#include "Game.h"
#include "ShrinkDownFX.h"
#include "BrickDebrisFX.h"
#include "HitStarFX.h"
#include "SmokeSpotFX.h"
#include "ScoreFX.h"
#include "RewardFX.h"
#include "DieFX.h"

void EffectPool::Initialization()
{
	for (int i = 0; i < N_FX; ++i)
		for (int j = 0; j < INIT_QUANTITIES[i]; ++j)
			CreateFX(INIT_FX_NAMES[i], VectorZero(), true);

	DebugOut(L"FXpool init done\n");
}

FXObject* EffectPool::CreateFX(std::string fxName, Vector2 position, bool expandPoolOnly)
{
	auto scene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();
	if (scene == nullptr) return nullptr;

	ObjectPool* pool;
	if (pools.find(fxName) != pools.end())
		pool = pools.at(fxName);
	else
		pool = new ObjectPool(), pools.insert(std::make_pair(fxName, pool));

	if (pool->IsEmpty() || expandPoolOnly)
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
		else if (fxName.compare("fx-mario-die") == 0)
			fx = Instantiate<DieFX>();
		else if (fxName.compare("fx-brick-debris") == 0)
			fx = Instantiate<BrickDebrisFX>();
		else if (fxName.compare("fx-hit-star") == 0)
			fx = Instantiate<HitStarFX>();
		else if (fxName.compare("fx-smoke-spot") == 0)
			fx = Instantiate<SmokeSpotFX>();
		else if (fxName.compare("fx-score") == 0)
			fx = Instantiate<ScoreFX>();
		else if (fxName.compare("fx-reward") == 0)
			fx = Instantiate<RewardFX>();
		
		DebugOut(L"New spawn\n");

		scene->AddObject(fx);
		fx->SetPosition(position);
		fx->SetStartPosition(position);
		fx->Start();
		fx->SetPool(pool);
		if (expandPoolOnly) pool->Revoke(fx);
		return fx;
	}
	else
	{
		FXObject* fx = static_cast<FXObject*>(pool->Instantiate());
		fx->SetPool(pool);
		fx->SetPosition(position);
		fx->SetStartPosition(position);
		fx->Start();
		if (scene->GetGrid() != nullptr) scene->GetGrid()->UpdateObject(fx);
		DebugOut(L"Old spawn\n");
		return fx;
	}
}
