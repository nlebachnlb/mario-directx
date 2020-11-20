#pragma once
#include "FXObject.h"
class BrickDebrisFX : public FXObject
{
public:
	void Awake() override;
	void Start() override;
	void Update() override;

private:
	Camera* mainCamera;
};

