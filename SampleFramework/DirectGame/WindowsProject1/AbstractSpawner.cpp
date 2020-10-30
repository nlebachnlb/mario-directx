#include "AbstractSpawner.h"

AbstractSpawner::AbstractSpawner()
{
	pool = new ObjectPool();
	Initialization();
}

AbstractSpawner::~AbstractSpawner()
{
	delete pool;
}

void AbstractSpawner::Initialization()
{
}

GameObject AbstractSpawner::Spawn(Vector2 position, bool reset)
{
	auto object = pool->Instantiate();
	if (reset) object->Start();
	object->SetPosition(position);
	return object;
}

SpawnPrototype* AbstractSpawner::GetPrototypeAt(int id)
{
	if (prototypes.find(id) != prototypes.end())
		return prototypes.at(id);
	return nullptr;
}

void AbstractSpawner::AddPrototype(int id, SpawnPrototype* prototype)
{
	prototypes.insert(std::make_pair(id, prototype));
}

ObjectPool* AbstractSpawner::GetPool()
{
	return this->pool;
}
