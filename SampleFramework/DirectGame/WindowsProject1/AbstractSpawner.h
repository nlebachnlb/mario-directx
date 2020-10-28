#pragma once
#include "Service.h"
#include "ObjectPool.h"
#include "Transform.h"

struct SpawnPrototype
{
	Vector2 startPosition;
	GameObject gameObject;

	SpawnPrototype(Vector2 position, GameObject object)
	{
		startPosition = position;
		gameObject = object;
	}

	static const SpawnPrototype Empty()
	{
		return SpawnPrototype{ VectorZero(), nullptr };
	}
};

class AbstractSpawner : public Service
{
public:
	AbstractSpawner();
	virtual ~AbstractSpawner();

	virtual void Initialization() override;

	GameObject Spawn(Vector2 position, bool reset = true);
	SpawnPrototype* GetPrototypeAt(int id);
	void AddPrototype(int, SpawnPrototype* prototype);

	ObjectPool* GetPool();

protected:
	ObjectPool* pool;
	std::unordered_map<int, SpawnPrototype*> prototypes;
};

