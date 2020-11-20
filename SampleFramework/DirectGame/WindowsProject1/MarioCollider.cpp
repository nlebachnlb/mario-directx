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

	auto canPassThrough = collisions.size() > 0 &&
		(TagUtils::EnemyTag(collisions.at(0)->collider->GetGameObject()->GetTag()) ||
		TagUtils::PowerupTag(collisions.at(0)->collider->GetGameObject()->GetTag()));
	if (canPassThrough == false)
	{
		if (ny != 0)
		{
			if (rigidbody->GetGravity() == 0)
			{
				velocity.y = -1 * Mathf::Sign(velocity.y) * rigidbody->GetMaterial().bounciness.y;
				// dvy = -1 * Mathf::Sign(dvy) * rigidbody->GetMaterial().bounciness.y * Game::DeltaTime();
				rigidbody->SetVelocity(&velocity);
			}
			else
			{
				if (nx == 0)
				{
					velocity.y = -1 * Mathf::Sign(velocity.y) * rigidbody->GetMaterial().bounciness.y;
					// dvy = -1 * Mathf::Sign(dvy) * rigidbody->GetMaterial().bounciness.y * Game::DeltaTime();
					rigidbody->SetVelocity(&velocity);
				}
			}
		}

		if (nx != 0
			&& collisions.size() > 0 && collisions.at(0)->collider->GetGameObject()->GetTag() == ObjectTags::Solid
			)
		{
			velocity.x = -1 * Mathf::Sign(velocity.x) * rigidbody->GetMaterial().bounciness.x;
			// dvx = -1 * Mathf::Sign(dvx) * rigidbody->GetMaterial().bounciness.x * Game::DeltaTime();
			rigidbody->SetVelocity(&velocity);
		}
	}

	VerticalCollisionProcess(collisions);
	HorizontalCollisionProcess(collisions);
}

void MarioCollider::BlockPosition(vector<CollisionEvent*>& collisions, float& min_tx, float& min_ty, float& nx, float& ny)
{
	bool canPassThrough = collisions.size() > 0 &&
		(TagUtils::EnemyTag(collisions.at(0)->collider->GetGameObject()->GetTag()) ||
		TagUtils::PowerupTag(collisions.at(0)->collider->GetGameObject()->GetTag()));
	if (canPassThrough)
	{
		auto pos = gameObject->GetTransform().Position;
		pos.x += dvx;
		pos.y += (ny < 0 ? min_ty * dvy + ny * pushCoefficient : dvy);
		gameObject->SetPosition(pos);
	}
	else 
		Collider2D::BlockPosition(collisions, min_tx, min_ty, nx, ny);
}

void MarioCollider::VerticalCollisionProcess(std::vector<CollisionEvent*>& collisions)
{
	for (auto collision : collisions)
	{
		auto tag = collision->collider->GetGameObject()->GetTag();
		if (TagUtils::EnemyTag(tag) && Mathf::Abs(collision->collisionDirection.y) > 0.00001f)
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
						if (!readyToRun)
						{
							if (collision->collisionDirection.y < 0)
								mario->Jump(MARIO_JUMP_FORCE, true);
							shell->Run();
						}
						else
						{
							mario->HoldObject(shell);
							shell->GetRigidbody()->SetGravity(0);
							shell->GetColliders()->at(0)->Disable();
							shell->PassToHolder(mario);
						}
					}
				}
				break;
			}

			default:
			{
				auto enemy = (AbstractEnemy*)(collision->collider->GetGameObject());

				if (collision->collisionDirection.y < 0)
				{
					mario->Jump(MARIO_JUMP_FORCE, true);
					
					if (enemy != nullptr)
						enemy->OnDead(false);
				}
				else if (collision->collisionDirection.y > 0)
				{
					// Get Damaged
					if (!mario->IsInvincible())
						mario->OnDamaged(enemy);
					DebugOut(L"VerDam-");
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

				if (shell->IsRunning())
				{
					if (!mario->IsInvincible())
						mario->OnDamaged(shell);
					DebugOut(L"HorDam-");
				}

				// If Mario is running, he will hold the shell
				if (readyToRun)
				{
					// And the shell is not running
					if (shell->IsRunning() == false)
					{
						mario->HoldObject(shell);
						shell->GetRigidbody()->SetGravity(0);
						shell->GetColliders()->at(0)->Disable();
						shell->PassToHolder(mario);
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
				// Normally, Mario will get damaged when he collides with enemy horizontally
				auto enemy = (AbstractEnemy*)collision->collider->GetGameObject();
				if (!mario->IsInvincible())
					mario->OnDamaged(enemy);
				DebugOut(L"CasualHorDam-%d-\n", mario->IsInvincible() ? 1 : 0);
				break;
			}
			}
		}
	}
}
