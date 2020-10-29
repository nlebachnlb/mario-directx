#include "MarioCollider.h"
#include "Mathf.h"
#include "Game.h"
#include "AbstractEnemy.h"
#include "KoopasShell.h"

void MarioCollider::CollisionProcess(std::vector<CollisionEvent*>& collisions, 
	Rigidbody2D* rigidbody, Vector2& velocity, 
	int mintx, int minty, int nx, int ny)
{
	if (mario == nullptr) 
		mario = dynamic_cast<CMario*>(gameObject);

	Collider2D::CollisionProcess(collisions, rigidbody, velocity, mintx, minty, nx, ny);

	for (auto collision : collisions)
	{
		auto tag = collision->collider->GetGameObject()->GetTag();
		if (collision->collisionDirection.y < 0 &&
			TagUtils::EnemyTag(tag))
		{
			switch (tag)
			{
			case ObjectTags::KoopasShell:
			{
				auto shell = (KoopasShell*)(collision->collider->GetGameObject());
				if (shell != nullptr)
				{
					shell->SetFacing(mario->GetFacing());
					if (shell->IsRunning())
					{
						shell->StopRunning();
						mario->Jump(MARIO_JUMP_FORCE, true);
					}
					else
					{
						shell->Run();
					}
				}
				break;
			}
			default:
			{
				mario->Jump(MARIO_JUMP_FORCE, true);
				auto enemy = (AbstractEnemy*)(collision->collider->GetGameObject());
				if (enemy != nullptr)
					enemy->OnDead(false);
				break;
			}
			}
		}
	}
}
