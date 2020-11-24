#pragma once
#include "ObjectPool.h"
#include "FXObject.h"
#include <unordered_map>
#include "Service.h"

const int N_FX = 1;
const std::string INIT_FX_NAMES[N_FX] = { "fx-brick-debris" };
const int INIT_QUANTITIES[N_FX] = { 40 };

class EffectPool : public Service
{
public:
	void Initialization() override;
	FXObject* CreateFX(std::string fxName, Vector2 position, bool expandPoolOnly = false);
protected:
	std::unordered_map<std::string, ObjectPool*> pools;
};

