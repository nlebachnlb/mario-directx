#pragma once
#include "GameObject.h"

const int MOVEMENT_DURATION = 800;
const Vector2 DESTINATION(412, 200);

class IntroTitle : public CGameObject
{
public:
	void Awake() override;
	void Start() override;
	void Update() override;
	void Render(Vector2 translation) override;

	void SetVersion(GameObject version);

private:
	Sprite sprites[3];
	int animStep, timer;
	GameObject version;
};

