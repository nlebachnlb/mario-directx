#pragma once
#include "GameObject.h"

const int REWARD_CHANGE_TIME = 70;
const Vector2 REWARD_BBOX(48, 48);

class LevelReward : public CGameObject
{
public:
	void Awake() override;
	void Start() override;
	void PreRender() override;
	void Render(Vector2 translation) override;

	void OnOverlapped(Collider2D* self, Collider2D* other) override;

private:
	Sprite items[3];
	int timer, reward;
};

