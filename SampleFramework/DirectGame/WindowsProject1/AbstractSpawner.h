#pragma once
#include "Service.h"
#include "ObjectPool.h"
#include "Transform.h"
#include "AbstractEnemy.h"

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
	virtual AbstractEnemy* Spawn(std::string name, Vector2 position, bool reset = true) = 0;
	SpawnPrototype* GetPrototypeAt(int id);
	void AddPrototype(int, SpawnPrototype* prototype);
	ObjectPool* GetPool(std::string name);

	// ObjectPool* GetPool(std::string name);

protected:
	std::unordered_map<std::string, ObjectPool*> pools;
	std::unordered_map<int, SpawnPrototype*> prototypes;
};
