#pragma once
#include "ObjectPool.h"
#include "FXObject.h"
#include <unordered_map>
#include "Service.h"

class EffectPool : public Service
{
public:
	void Initialization() override;
	FXObject* CreateFX(std::string fxName, Vector2 position);
protected:
	std::unordered_map<std::string, ObjectPool*> pools;
};

