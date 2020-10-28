#include "AbstractSpawner.h"

AbstractSpawner::AbstractSpawner()
{
	pool = new ObjectPool();
}

AbstractSpawner::~AbstractSpawner()
{
	delete pool;
}

GameObject AbstractSpawner::Spawn(Vector2 position, bool reset)
{
	auto object = pool->Instantiate();
	if (reset) object->Start();
	object->SetPosition(position);
	return object;
}
