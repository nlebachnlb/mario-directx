#pragma once
#include "AbstractSpawner.h"
#include "KoopasShell.h"

class KoopaSpawner : public AbstractSpawner
{
public:
	KoopaSpawner() : AbstractSpawner() { shells = new ObjectPool(); }
	void Initialization() override;
	KoopasShell* InstantiateShell(Vector2 position, bool reset = true);
private:
	ObjectPool* shells;
};

