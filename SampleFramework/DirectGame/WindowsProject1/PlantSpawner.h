#pragma once
#include "AbstractSpawner.h"
class PlantSpawner : public AbstractSpawner
{
public:
	AbstractEnemy* Spawn(std::string name, Vector2 position, bool reset = true) override;
};

