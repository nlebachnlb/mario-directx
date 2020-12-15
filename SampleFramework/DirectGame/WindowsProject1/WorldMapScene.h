#pragma once
#include "Scene.h"
#include "MarioLocator.h"
class WorldMapScene : public Scene
{
public:
	void Load() override;

	MarioLocator* GetMarioLocator();

private:
	MarioLocator* objMario;
};

