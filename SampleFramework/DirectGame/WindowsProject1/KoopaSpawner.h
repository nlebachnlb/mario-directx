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
	KoopaSpawner() : AbstractSpawner() { redShells = new ObjectPool(); greenShells = new ObjectPool();  }
	void Initialization() override;
	KoopasShell* InstantiateShell(Vector2 position, KoopasShellType shellType, bool reset = true);
	AbstractEnemy* Spawn(std::string name, Vector2 position, bool reset = true, int cellx = -1, int celly = -1);
private:
	ObjectPool* redShells, *greenShells;
};

