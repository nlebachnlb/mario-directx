#pragma once
#include "AbstractEnemy.h"

const Vector2 KOOPAS_SHELL_BBOX(16 * 3 - 8, 16 * 3 - 10);
const float KOOPAS_SHELL_MOVING_SPEED = 0.2f;
const PhysicMaterial KOOPAS_SHELL_PHYSIC_MATERIAL(Vector2(KOOPAS_SHELL_MOVING_SPEED, 0), 0);
const float KOOPAS_SHELL_GRAVITY = 0.00093f;
const int KOOPAS_SHELL_DEAD_TIME = 500; // miliseconds
const float KOOPAS_SHELL_DEFLECTION_ON_SHOT = -0.4f;

class KoopasShell : public AbstractEnemy
{
public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Movement() override;
	virtual void InitAnimations() = 0;
	virtual void OnDead(bool oneHit) override;

	void SetFacing(int facing);
	void Run();
	void StopRunning();

	bool IsRunning();

protected:
	Vector2 GetBoxSize() override;

private:
	int facing;
	bool running;
};

