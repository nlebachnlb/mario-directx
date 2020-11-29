#pragma once
#include "Transform.h"
#include "PhysicMaterial.h"
class Rigidbody2D
{
public:
	Rigidbody2D();

	void Update();

	void SetVelocity(Vector2* velocity);
	Vector2 GetVelocity();

	void SetGravity(float gravity);
	float GetGravity();

	bool IsDynamic();
	void SetDynamic(bool dynamic);

	void SetDrag(Vector2 drag);
	Vector2 GetDrag();

	void SetMass(float mass);
	float GetMass();

	void SetAcceleration(float acceleration);
	float GetAcceleration();

	void SetMaterial(PhysicMaterial material);
	PhysicMaterial GetMaterial();

	void SetFallLimitVelocity(float limit);
	float GetFallLimitVelocity();

	void PassDataTo(Rigidbody2D* target);
	void GenerateDragForce();

protected:
	PhysicMaterial material;
	Vector2 velocity;
	float gravity;
	bool isDynamic;
	Vector2 drag;
	float acceleration;
	float mass;
	float fallLimit;
};

