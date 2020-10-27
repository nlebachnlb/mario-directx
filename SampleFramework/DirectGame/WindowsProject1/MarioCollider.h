#pragma once
#include "Collider2D.h"
#include "CMario.h"

class MarioCollider : public Collider2D
{
public:
	MarioCollider() : Collider2D() { mario = nullptr; }
protected:
	virtual void CollisionProcess(std::vector<CollisionEvent*>& collisions, 
		Rigidbody2D* rigidbody, Vector2& velocity, 
		int mintx, int minty, int nx, int ny) override;

private:
	CMario* mario;
};

