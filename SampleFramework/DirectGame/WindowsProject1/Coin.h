#pragma once
#include "AbstractItem.h"
class Coin : public AbstractItem
{
public:
	void Awake() override;
	void Start() override;
	void LateUpdate() override;
	void PreRender() override;
};

