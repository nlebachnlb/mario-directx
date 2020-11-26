#pragma once
#include "CMario.h"
#include "RaccoonAttackBox.h"
#include "Scene.h"

const int RACCOON_FLOAT_TIME = 500; // miliseconds
const int RACCOON_FLY_TIME = 4000; // miliseconds = 4 seconds
const float RACCOON_FLY_VELOCITY = -0.3f;
const int RACCOON_FEVER_TIME = 100;
const float RACCOON_ATTACK_TIME = 200;

class RaccoonMario : public CMario
{
public:
	void Awake() override;
	void Start() override;
	void OnAnimationEnd() override;
	void OnKeyDown(int keyCode) override;
	void Exit() override;

	void RegisterToScene(Scene* scene);
protected:
	void InitAnimations() override;
	void MovementAnimation() override;
	void JumpingAnimation() override;
	void LateUpdate() override;
	void OnCollisionEnter(Collider2D* selfCollider, vector<CollisionEvent*> collisions) override;
private:
	bool attacking;
	int physicalAttacking;
	bool floating, pushing;
	int flying;
	int lastFloatingTime, lastFlyingTime, lastPushingTime, lastAttackingTime;
	RaccoonAttackBox* attackBox;
};

