#pragma once
#include "Scene.h"
#include "GameMap.h"

class ExampleScene : public Scene
{
public:
	void Load();

private:
	GameMap* worldMap;
};

