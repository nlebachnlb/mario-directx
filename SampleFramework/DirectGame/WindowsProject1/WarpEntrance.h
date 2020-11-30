#pragma once
#include "GameObject.h"
#include "WarpUtils.h"

class WarpEntrance : public CGameObject
{
public:
	void Awake() override;

	void SetWarpDirection(WarpDirection direction);
	WarpDirection GetWarpDirection();

private:
	WarpDirection warpDirection;
};

