#include "Rigidbody2D.h"
#include "Utils.h"

Rigidbody2D::Rigidbody2D()
{
	this->velocity = VectorZero();
	this->gravity = 0.0f;
	this->acceleration = 0.0f;
	this->drag = VectorZero();
	this->mass = 1.0f;
	this->material = PhysicMaterial::Empty();
	this->fallLimit = -1;
}

void Rigidbody2D::Update()
{
}

void Rigidbody2D::SetVelocity(Vector2* velocity)
{
	this->velocity.x = velocity->x;
	this->velocity.y = velocity->y;
}

Vector2 Rigidbody2D::GetVelocity()
{
	return this->velocity;
}

void Rigidbody2D::SetGravity(float gravity)
{
	this->gravity = gravity;
}

float Rigidbody2D::GetGravity()
{
	return this->gravity;
}

bool Rigidbody2D::IsDynamic()
{
	return this->isDynamic;
}

void Rigidbody2D::SetDynamic(bool dynamic)
{
	this->isDynamic = dynamic;
}

void Rigidbody2D::SetDrag(Vector2 drag)
{
	this->drag = drag;
}

Vector2 Rigidbody2D::GetDrag()
{
	return this->drag;
}

void Rigidbody2D::SetMass(float mass)
{
	this->mass = mass;
}

float Rigidbody2D::GetMass()
{
	return this->mass;
}

void Rigidbody2D::SetAcceleration(float acceleration)
{
	this->acceleration = acceleration;
}

float Rigidbody2D::GetAcceleration()
{
	return this->acceleration;
}

void Rigidbody2D::SetMaterial(PhysicMaterial material)
{
	this->material = material;
}

PhysicMaterial Rigidbody2D::GetMaterial()
{
	return this->material;
}

void Rigidbody2D::SetFallLimitVelocity(float limit)
{
	this->fallLimit = limit;
}

float Rigidbody2D::GetFallLimitVelocity()
{
	return this->fallLimit;
}

void Rigidbody2D::PassDataTo(Rigidbody2D* target)
{
	target->acceleration = this->acceleration;
	target->drag = this->drag;
	target->fallLimit = this->fallLimit;
	target->velocity = this->velocity;
	target->gravity = this->gravity;
	target->material = this->material;
	target->isDynamic = this->isDynamic;
}
