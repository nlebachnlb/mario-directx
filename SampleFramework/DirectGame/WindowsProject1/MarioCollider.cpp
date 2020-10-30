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

	VerticalCollisionProcess(collisions);
	HorizontalCollisionProcess(collisions);
}

void MarioCollider::VerticalCollisionProcess(std::vector<CollisionEvent*>& collisions)
{
	for (auto collision : collisions)
	{
		auto tag = collision->collider->GetGameObject()->GetTag();
		if (TagUtils::EnemyTag(tag))
		{
			switch (tag)
			{
			case ObjectTags::KoopasShell:
			{
				auto readyToRun = mario->IsReadyToRun();
				auto shell = (KoopasShell*)(collision->collider->GetGameObject());
				if (shell != nullptr)
				{
					shell->SetFacing(Mathf::Sign(shell->GetTransform().Position.x - mario->GetTransform().Position.x));
					if (shell->IsRunning() && collision->collisionDirection.y < 0)
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
				if (collision->collisionDirection.y < 0)
				{
					mario->Jump(MARIO_JUMP_FORCE, true);
					auto enemy = (AbstractEnemy*)(collision->collider->GetGameObject());
					if (enemy != nullptr)
						enemy->OnDead(false);
				}
				else if (collision->collisionDirection.y > 0)
				{
					// Get Damaged
				}
				break;
			}
			}
		}
	}
}

void MarioCollider::HorizontalCollisionProcess(std::vector<CollisionEvent*>& collisions)
{
	for (auto collision : collisions)
	{
		auto tag = collision->collider->GetGameObject()->GetTag();
		if (collision->collisionDirection.x != 0 &&
			TagUtils::EnemyTag(tag))
		{
			switch (tag)
			{
			case ObjectTags::KoopasShell:
			{
				auto readyToRun = mario->IsReadyToRun();
				auto shell = (KoopasShell*)(collision->collider->GetGameObject());

				// If Mario is running, he will hold the shell
				if (readyToRun)
				{
					// And the shell is not running
					if (shell->IsRunning() == false)
					{
						mario->HoldObject(shell);
						// shell->GetColliders()->at(0)->Disable();
						shell->GetRigidbody()->SetGravity(0);
						shell->PassToHolder(mario);
					}
					// Otherwise, he gets damaged
					else
					{

					}
				}
				// Otherwise, he kicks it
				else
				{
					shell->SetFacing(mario->GetFacing());
					if (shell->IsRunning() == false)
					{
						shell->Run();
					}
				}
				break;
			}
			default:
			{
				// Normally, Mario will get damaged when he collides with enemy horizonatally
				
				break;
			}
			}
		}
	}
}
