#pragma once
#include "GameObject.h"

const float CURTAIN_START = 642;

class SceneAnimator : public CGameObject
{
public:
	void Awake() override;
	void Start() override;
	void Update() override;
	void Render(Vector2 translation) override;

private:
	std::unordered_map<std::string, GameObject> objects;
	Sprite sprites[4];
	int cutOrder;

	int elapsedTime;

	Vector2 curtainPos;
};

