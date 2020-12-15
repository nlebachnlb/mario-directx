#pragma once
#include "GameObject.h"
#include "GraphNode.h"

class MapNode : public CGameObject
{
public:
	virtual void Awake() override;

	virtual void InitAnimations() = 0;
};

