#include "MarioCollider.h"
#include "Mathf.h"
#include "Game.h"

void MarioCollider::CollisionProcess(std::vector<CollisionEvent*>& collisions, 
	Rigidbody2D* rigidbody, Vector2& velocity, 
	int mintx, int minty, int nx, int ny)
{
	if (mario == nullptr) mario = dynamic_cast<CMario*>(gameObject);

	if (ny != 0)
	{
		if (rigidbody->GetGravity() == 0)
		{
			velocity.y = -1 * Mathf::Sign(velocity.y) * rigidbody->GetMaterial().bounciness.y;
			dvy = -1 * Mathf::Sign(dvy) * rigidbody->GetMaterial().bounciness.y * Game::DeltaTime();
			rigidbody->SetVelocity(&velocity);
		}
		else
		{
			if (nx == 0)
			{
				velocity.y = -1 * Mathf::Sign(velocity.y) * rigidbody->GetMaterial().bounciness.y;
				dvy = -1 * Mathf::Sign(dvy) * rigidbody->GetMaterial().bounciness.y * Game::DeltaTime();
				rigidbody->SetVelocity(&velocity);
			}
		}
	}

	if (nx != 0)
	{
		velocity.x = -1 * Mathf::Sign(velocity.x) * rigidbody->GetMaterial().bounciness.x;
		dvx = -1 * Mathf::Sign(dvx) * rigidbody->GetMaterial().bounciness.x * Game::DeltaTime();
		rigidbody->SetVelocity(&velocity);
	}
}
