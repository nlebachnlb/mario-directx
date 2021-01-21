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
	void BlockPosition(vector<CollisionEvent*>& collisions, float& min_tx, float& min_ty, float& nx, float& ny);

private:
	CMario* mario;
	void VerticalCollisionProcess(std::vector<CollisionEvent*>& collisions);
	void HorizontalCollisionProcess(std::vector<CollisionEvent*>& collisions);
	friend class CMario;
};

