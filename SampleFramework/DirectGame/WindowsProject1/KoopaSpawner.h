#pragma once
#include "AbstractSpawner.h"
#include "KoopasShell.h"

enum class KoopasShellType
{
	Red, Green
};

class KoopaSpawner : public AbstractSpawner
{
public:
	KoopaSpawner() : AbstractSpawner() { shells = new ObjectPool(); }
	void Initialization() override;
	KoopasShell* InstantiateShell(Vector2 position, KoopasShellType shellType, bool reset = true);
private:
	ObjectPool* shells;
};

