#include "CMario.h"
#include "Game.h"
#include "AnimationDatabase.h"
#include "Consts.h"
#include "Mathf.h"
#include "MarioCollider.h"

void CMario::Awake()
{
	SetTag(ObjectTags::BigMario);
	DebugOut(L"Mario Awake\n");

	InitAnimations();
	 
	MarioCollider* collider = new MarioCollider();
	collider->SetBoxSize(MARIO_BBOX);
	collider->AttachToEntity(this);
	collider->SetName("Normal");
	this->colliders->push_back(collider); 

	canCrouch = true;
	input = nullptr;
}

void CMario::Start()
{
	SetState("Idle");

	this->rigidbody->SetGravity(MARIO_GRAVITY);
	this->rigidbody->SetDynamic(true);
	this->rigidbody->SetAcceleration(MARIO_WALK_ACCELERATION);
	this->rigidbody->SetDrag(Vector2(MARIO_WALK_DRAG_FORCE, 0.0f));
	rigidbody->GetMaterial().AddBypassTag(ObjectTags::FriendlyProjectiles);
	rigidbody->GetMaterial().AddBypassTag(ObjectTags::BigMario);
	rigidbody->GetMaterial().AddBypassTag(ObjectTags::PowerupMario);
	rigidbody->GetMaterial().AddBypassTag(ObjectTags::SmallMario);

	physicState = { MovingStates::Idle, JumpingStates::Stand };
	facing = 1;
	skid = false;

	// Not used yet
	canSkid = true;

	canHighJump = false;
	deflect = false;

	pMeter = 0.0f;
	runningRestriction = false;

	feverState = 0;
	feverTime = MARIO_FEVER_TIME;
	lastFeverTime = 0;

	hold = false;
	heldInHandsObject = nullptr;
}

void CMario::Update()
{
	if (input == nullptr)
		input = Game::GetInstance().GetService<InputHandler>();

	auto velocity = rigidbody->GetVelocity();
	previousVelocity = velocity;
	prevPhysicState = physicState;
	prevTargetVelocityX = targetVelocityX;
	auto speed = velocity.x;

	// Horizontal movement
	if (input->GetKeyDown(marioKeySet.Left) || input->GetKeyDown(marioKeySet.Right))
	{
		// Accelerate velocity based on moving states
		if (input->GetKeyDown(marioKeySet.Attack) && runningRestriction == false)
		{
			physicState.movement = MovingStates::Run;

			if (skid == false)
				rigidbody->SetAcceleration(MARIO_RUN_ACCELERATION);
			else
				rigidbody->SetAcceleration(MARIO_SKID_ACCELERATION);

			rigidbody->SetDrag(Vector2(MARIO_RUN_DRAG_FORCE, rigidbody->GetDrag().y));
		}
		else
		{
			if (feverState == 2) feverState = 0;
			physicState.movement = MovingStates::Walk;
			rigidbody->SetAcceleration(MARIO_WALK_ACCELERATION);
			rigidbody->SetDrag(Vector2(MARIO_WALK_DRAG_FORCE, rigidbody->GetDrag().y));
		}

		auto constSpeed = physicState.movement == MovingStates::Run ? MARIO_RUN_SPEED : MARIO_WALK_SPEED;
		targetVelocityX = constSpeed;
		
		if (input->GetKeyDown(marioKeySet.Left))
			targetVelocityX = -1 * constSpeed;
		else if (input->GetKeyDown(marioKeySet.Right))
			targetVelocityX = +1 * constSpeed;
		
		if (Mathf::Abs(speed - targetVelocityX) > rigidbody->GetAcceleration() * Game::DeltaTime())
		{
			if (speed < targetVelocityX)
				speed += rigidbody->GetAcceleration() * Game::DeltaTime();
			else
				speed -= rigidbody->GetAcceleration() * Game::DeltaTime();
		}
		else
		{
			speed = targetVelocityX;
		}

		// DebugOut(L"moving state: %d, %d\n", prevPhysicState.movement, physicState.movement);
		velocity.x = speed;
		facing = Mathf::Sign(velocity.x);

		SkidDetection(velocity);
	}
	else
	{
		// If Mario stops moving, the drag force will reduce the remaining speed
		if (Mathf::Abs(speed) > rigidbody->GetDrag().x * Game::DeltaTime()) 
			speed = Mathf::Abs(speed) - (rigidbody->GetDrag().x * Game::DeltaTime());
		else
		{
			speed = 0.0f;

			// If Mario does not move anymore, he will be idle
			if (physicState.movement != MovingStates::Idle)
				physicState.movement = MovingStates::Idle;
		}

		velocity.x = facing * speed;
		skid = false;
	}

	run = Mathf::Abs(Mathf::Abs(velocity.x)) > MARIO_RUN_SPEED * 0.85f;
	maxRun = Mathf::Abs(Mathf::Abs(velocity.x)) > MARIO_RUN_SPEED * 0.85f;
	rigidbody->SetVelocity(&velocity);

	// If Mario runs at max speed, the P Meter starts increasing
	if (physicState.movement == MovingStates::Run && Mathf::Abs(velocity.x) > MARIO_RUN_SPEED * 0.15f &&
		pMeter < PMETER_MAX + 1 && physicState.jump == JumpingStates::Stand && 
		feverState != 2)
	{
		pMeter = Mathf::Clamp(pMeter + PMETER_STEP * Game::DeltaTime(), 0.0f, PMETER_MAX + 1);
		if (feverState != -1) feverState = 1;
		// DebugOut(L"[Fever] --power: %f\n", pMeter);
	}
	else if (feverState != 2 && feverState != -1)
		feverState = 0;

	// Fever mode processing
	if (pMeter >= PMETER_MAX && feverState == 1)
	{
		feverState = 2;
		lastFeverTime = GetTickCount();
		// DebugOut(L"[Fever] start\n");
	}
	else if (pMeter > 0 && feverState <= 0)
	{
		pMeter = Mathf::Clamp(pMeter - PMETER_STEP * 2.0f * Game::DeltaTime(), 0.0, PMETER_MAX);
	}

	if (feverState == 2)
	{
		pMeter = PMETER_MAX;
		if (GetTickCount() - lastFeverTime > feverTime)
		{
			feverState = 0;
			// DebugOut(L"[Fever] done\n");
		}
	}

	// Keep Mario inside Camera bounds
	auto mainCamera = Game::GetInstance().GetService<SceneManager>()->GetActiveScene()->GetMainCamera();
	transform.Position.x = Mathf::Clamp(transform.Position.x, mainCamera->GetPosition().x + MARIO_BBOX.x, mainCamera->GetPosition().x + mainCamera->GetViewportSize().x - MARIO_BBOX.x);

	// Vertical Movement
	if (physicState.jump == JumpingStates::Jump)
	{
		velocity = rigidbody->GetVelocity();
		auto jumpForce = MARIO_JUMP_FORCE;
		highJump = false;

		auto onAir = Mathf::InRange
		(
			velocity.y,
			-(feverState == 2 ? MARIO_SUPER_JUMP_FORCE : MARIO_HIGH_JUMP_FORCE),
			-0.5f * MARIO_JUMP_FORCE
		);

		onAir = onAir || deflect;

		if (input->GetKeyDown(marioKeySet.Jump) && canHighJump && onAir)
		{
			jumpForce = MARIO_HIGH_JUMP_FORCE;
			if ((feverState == 2 && run) || deflect)
				jumpForce = MARIO_SUPER_JUMP_FORCE;
			highJump = true;
		}
		
		// Provide force to push Mario up
		if (velocity.y > -jumpForce && velocity.y < 0 && canHighJump)
		{
			rigidbody->SetGravity(0.0f);
			velocity.y -= MARIO_PUSH_FORCE * Game::DeltaTime();
			rigidbody->SetVelocity(&velocity);
			// DebugOut(L"VEL_Y: %f, %f\n", velocity.y, -jumpForce);
		}
		else
		{
			// Reach maximum distance, get ready to fall
			velocity.y = -1 * jumpForce;
			rigidbody->SetVelocity(&velocity);
			physicState.jump = JumpingStates::High;
			rigidbody->SetGravity(MARIO_GRAVITY);
			// DebugOut(L"MAXIMUM: %f\n", velocity.y);
		}
	}
	else if (physicState.jump == JumpingStates::High)
	{
		velocity = rigidbody->GetVelocity();
		
		// After reaching the max jump distance, fall down
		if (velocity.y > 0)
		{
			canHighJump = false;
			physicState.jump = JumpingStates::Fall;
		}
	}
	else if (physicState.jump == JumpingStates::Fall)
	{
		// Fall and land on ground, switch to stand
		if (onGround)
			physicState.jump = JumpingStates::Stand;
	}
	else if (physicState.jump == JumpingStates::Stand)
	{
		// Fall down from a higher place
		// distance = v*t + 0.5at^2
		auto distance = rigidbody->GetVelocity().y * Game::DeltaTime() + 0.5f * rigidbody->GetGravity() * Game::DeltaTime() * Game::DeltaTime();
		if (distance > MARIO_MIN_VDISTANCE)
		{
			onGround = false;
			physicState.jump = JumpingStates::Fall;
		}
	}

	if (canCrouch && !hold) CrouchDetection(input);
	HoldProcess();
}

void CMario::PreRender()
{
	// This update will be used for Animation updating
	if (physicState.jump == JumpingStates::Stand)
		MovementAnimation();
	else
		JumpingAnimation();

	SetScale(Vector2(1 * facing, 1));
}
 
void CMario::Entrance()
{
	this->enabled = true;
}

void CMario::Execute()
{
}

void CMario::Exit()
{
	this->enabled = false;
}

int CMario::GetFacing()
{
	return this->facing;
}

int CMario::SetFacing(int facing)
{
	return this->facing = facing;
}

void CMario::Jump(float force, bool deflect)
{
	rigidbody->SetVelocity(&Vector2(rigidbody->GetVelocity().x, -force));
	physicState.jump = JumpingStates::Jump;

	onGround = false;
	canHighJump = true;
	this->deflect = deflect;

	// DebugOut(L"Can high jump: %d, %f\n", canHighJump ? 1 : 0, rigidbody->GetVelocity().y);
}

bool CMario::IsReadyToRun()
{
	return physicState.movement == MovingStates::Run;
}

void CMario::HoldObject(Holdable* holdableObj)
{
	hold = true;
	this->heldInHandsObject = holdableObj;
	holdableObj->PassToHolder(this);
}

void CMario::OnKeyDown(int keyCode)
{
	if (keyCode == marioKeySet.Jump && onGround && physicState.jump == JumpingStates::Stand)
	{
		Jump();
	}
}

void CMario::OnKeyUp(int keyCode)
{
	if (keyCode == marioKeySet.Jump)
	{
		canHighJump = false;
		// DebugOut(L"Can high jump: %d\n", canHighJump ? 1 : 0);
	}
}

void CMario::OnCollisionEnter(Collider2D* selfCollider, vector<CollisionEvent*> collisions)
{
	for (auto collision : collisions)
	{
		auto collider = collision->collider;
		if (collider->GetGameObject()->GetTag() == ObjectTags::Solid ||
			collider->GetGameObject()->GetTag() == ObjectTags::GhostPlatform
			)
		{
			// DebugOut(L"Hit Solid: %f\n", collision->collisionDirection.y);
			if (collision->collisionDirection.y < 0 &&
				collision->collisionDirection.x <= 0.00001f)
			{
				onGround = true;
				physicState.jump = JumpingStates::Stand;
			}

			if (collision->collisionDirection.x != 0)
			{
				rigidbody->SetVelocity(&Vector2(0, rigidbody->GetVelocity().y));
			}
		}
	}
}

void CMario::OnTriggerEnter(Collider2D* selfCollider, vector<CollisionEvent*> collisions)
{
}

void CMario::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();

	AddAnimation("Run",		animations->Get("ani-big-mario-high-speed"));
	AddAnimation("Walk",	animations->Get("ani-big-mario-walk"));
	AddAnimation("Jump",	animations->Get("ani-big-mario-jump"));
	AddAnimation("Fly" ,	animations->Get("ani-big-mario-high-jump"));
	AddAnimation("Fall",	animations->Get("ani-big-mario-fall"));
	AddAnimation("Idle",	animations->Get("ani-big-mario-idle"));
	AddAnimation("Skid",	animations->Get("ani-big-mario-skid"));
	AddAnimation("Crouch",	animations->Get("ani-big-mario-crouch"));

	AddAnimation("HoldIdle",	animations->Get("ani-big-mario-hold-idle"));
	AddAnimation("HoldMove",	animations->Get("ani-big-mario-hold"));
	AddAnimation("Kick",		animations->Get("ani-big-mario-kick"));
}

void CMario::MovementAnimation()
{
	auto animation = GetState(currentState);

	if (physicState.movement == MovingStates::Idle)
	{
		if (hold)
			SetState("HoldIdle");
		else 
			SetState("Idle");
		animation->SetSpeedMultiplier(1.0f);
	}

	if (physicState.movement == MovingStates::Walk || physicState.movement == MovingStates::Run)
	{
		auto speed = Mathf::Abs(rigidbody->GetVelocity().x);

		if (run)
		{
			if (hold)
				SetState("HoldMove");
			else 
				SetState("Run");
		}
		else if (skid && !hold)
			SetState("Skid");
		else
		{
			if (hold)
				SetState("HoldMove");
			else
				SetState("Walk");
		}

		animation->SetSpeedMultiplier(Mathf::Clamp(speed / MARIO_WALK_SPEED, 1.0f, 4.0f));
	}

	if (physicState.movement == MovingStates::Crouch && !hold)
	{
		SetState("Crouch");
	}

	SetVisualRelativePosition(colliders->at(0)->GetLocalPosition());
}

void CMario::JumpingAnimation()
{
	if (currentState.compare("Fly") == 0 
		//|| currentState.compare("Jump") == 0 || currentState.compare("Fall") == 0
		)
		return;

	if (feverState == 2 && !hold)
	{
		SetState("Fly");
		return;
	}

	if (physicState.jump == JumpingStates::Jump || physicState.jump == JumpingStates::High)
	{
		if (hold)
			SetState("Fall");
		else 
			SetState("Jump");
	}
	else if (physicState.jump == JumpingStates::Fall)
	{
		SetState("Fall");
	}
}

void CMario::SkidDetection(Vector2 velocity)
{
	bool movementConstraint = (
			(
				physicState.movement == MovingStates::Walk &&
				Mathf::InRange(Mathf::Abs(velocity.x), MARIO_WALK_SPEED, MARIO_RUN_SPEED) == false
			)
			||
			physicState.movement == MovingStates::Run
		);

	bool facingConstraint = (
			(facing == 1 && previousVelocity.x > velocity.x) ||
			(facing == -1 && previousVelocity.x < velocity.x)
		);

	if (movementConstraint && facingConstraint && skid == false)
		skid = true;
	else if (Mathf::Sign(velocity.x) != Mathf::Sign(previousVelocity.x))
		skid = false;
}

void CMario::CrouchDetection(InputHandler* input)
{
	bool exitConditions =
		input->GetKeyDown(marioKeySet.Left) || input->GetKeyDown(marioKeySet.Right) ||
		physicState.jump != JumpingStates::Stand;

	if (input->GetKeyDown(marioKeySet.Crouch) && exitConditions == false)
	{
		colliders->at(0)->SetBoxSize(MARIO_SMALL_BBOX);
		Vector2 delta = Vector2(0, MARIO_BBOX.y - MARIO_SMALL_BBOX.y);
		colliders->at(0)->SetLocalPosition(delta * 0.5f);
		physicState.movement = MovingStates::Crouch;
	}
	else
	{
		colliders->at(0)->SetBoxSize(MARIO_BBOX);
		colliders->at(0)->SetLocalPosition(VectorZero());
	}
}

void CMario::HoldProcess()
{
	if (heldInHandsObject == nullptr) return;

	if (hold)
	{
		if (input->GetKeyDown(marioKeySet.Attack))
		{
			auto delta = 0.4f * (heldInHandsObject->GetColliderBox() + MARIO_BBOX);
			heldInHandsObject->SetHoldablePosition(Vector2(transform.Position.x + Mathf::Abs(delta.x) * facing, transform.Position.y));
			heldInHandsObject->SetHoldableFacing(facing);
		}
		else
		{
			hold = false;
			heldInHandsObject->Release();
		}
	}
}

bool CMario::IsStateTransition(MovingStates srcState, MovingStates dstState)
{
	return (prevPhysicState.movement == srcState && physicState.movement == dstState);
}
