#pragma once
#include "Service.h"
#include "ObjectPool.h"
#include "Transform.h"

struct SpawnPrototype
{
	Vector2 startPosition;
	GameObject gameObject;
};

class AbstractSpawner : public Service
{
public:
	AbstractSpawner();
	virtual ~AbstractSpawner();

	GameObject Spawn(Vector2 position, bool reset = true);

protected:
	ObjectPool* pool;
	std::unordered_map<std::string, SpawnPrototype&> prototypes;
};

