#pragma once
#include "AbstractSpawner.h"
class PlantSpawner : public AbstractSpawner
{
public:
	AbstractEnemy* Spawn(std::string name, Vector2 position, bool reset = true, int cellx = -1, int celly = -1) override;
};

