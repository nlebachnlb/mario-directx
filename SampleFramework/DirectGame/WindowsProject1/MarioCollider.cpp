#include "MarioCollider.h"
#include "Mathf.h"
#include "Game.h"

void MarioCollider::CollisionProcess(std::vector<CollisionEvent*>& collisions, 
	Rigidbody2D* rigidbody, Vector2& velocity, 
	int mintx, int minty, int nx, int ny)
{
	if (mario == nullptr) 
		mario = dynamic_cast<CMario*>(gameObject);

	Collider2D::CollisionProcess(collisions, rigidbody, velocity, mintx, minty, nx, ny);

	for (auto collision : collisions)
	{
		if (collision->collisionDirection.y < 0 &&
			TagUtils::EnemyTag(collision->collider->GetGameObject()->GetTag()))
			mario->Jump(MARIO_JUMP_FORCE, true);
	}
}
