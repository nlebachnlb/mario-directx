#pragma once
#include "AbstractEnemy.h"
#include "Holdable.h"

const Vector2 KOOPAS_SHELL_BBOX(16 * 3 - 8, 16 * 3 - 10);
const float KOOPAS_SHELL_MOVING_SPEED = 0.65f;
const PhysicMaterial KOOPAS_SHELL_PHYSIC_MATERIAL(Vector2(KOOPAS_SHELL_MOVING_SPEED, 0), 0);
const float KOOPAS_SHELL_GRAVITY = 0.0024f;
const int KOOPAS_SHELL_DEAD_TIME = 1000; // miliseconds
const float KOOPAS_SHELL_DEFLECTION_ON_SHOT = -0.95f;
const float KOOPAS_SHELL_HITBACK_SPEED = 0.11f;
const float KOOPAS_SHELL_DRAG_FORCE = 0.0001f;
const int KOOPAS_WITHDRAW_TIME = 4500;
const int WITHDRAW_AMPLITUDE = 3; // px
const int WITHDRAW_ROUNDTRIP_TIME = 60;

class KoopasShell : public AbstractEnemy, public Holdable
{
public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Movement() override;
	virtual void InitAnimations() = 0;
	virtual void OnWithdraw() = 0;
	virtual void OnDead(bool oneHit) override;
	virtual void OnCollisionEnter(Collider2D* selfCollider, vector<CollisionEvent*> collisions);
	virtual void OnOverlapped(Collider2D* selfCollider, Collider2D* otherCollider);
	virtual void OnOffScreen() override;

	// Implement holdable interface methods
	virtual Vector2 GetColliderBox() override;
	virtual void SetHoldablePosition(Vector2 position) override;
	virtual void OnRelease() override; 

	void SetFacing(int facing);
	void Run();
	void StopRunning();

	bool IsRunning();

protected:
	Vector2 GetBoxSize() override;

private:
	void StartWithdrawing();
	int facing;
	bool running;
	int timer, withdrawStep;
};

