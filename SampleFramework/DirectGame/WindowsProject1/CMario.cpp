#include "CMario.h"
#include "Game.h"
#include "AnimationDatabase.h"
#include "Consts.h"
#include "Mathf.h"

void CMario::Awake()
{
	SetTag(ObjectTags::BigMario);
	DebugOut(L"Mario Awake\n");

	InitAnimations();

	Collider2D* collider = new Collider2D();
	collider->SetBoxSize(MARIO_BBOX);
	collider->AttachToEntity(this);
	collider->SetName("Normal");
	this->colliders->push_back(collider); 

	canCrouch = true;
}

void CMario::Start()
{
	SetState("Idle");

	this->rigidbody->SetGravity(MARIO_GRAVITY);
	this->rigidbody->SetDynamic(true);
	this->rigidbody->SetAcceleration(MARIO_WALK_ACCELERATION);
	this->rigidbody->SetDrag(Vector2(MARIO_WALK_DRAG_FORCE, 0.0f));

	physicState = { MovingStates::Idle, JumpingStates::Stand };
	facing = 1;
	skid = false;

	// Not used yet
	canSkid = true;

	canHighJump = false;
	pMeter = 0.0f;
	runningRestriction = false;
}

void CMario::Update()
{
	auto input = Game::GetInstance().GetService<InputHandler>();
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
	if (maxRun && pMeter < PMETER_MAX + 1)
		pMeter = Mathf::Clamp(pMeter + PMETER_STEP * Game::DeltaTime(), 0.0f, PMETER_MAX + 1);
	else if (pMeter > 0)
		pMeter = Mathf::Clamp(pMeter - PMETER_STEP * Game::DeltaTime(), 0.0, PMETER_MAX);

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
			-(maxRun && pMeter >= PMETER_MAX ? MARIO_SUPER_JUMP_FORCE : MARIO_HIGH_JUMP_FORCE),
			-0.5f * MARIO_JUMP_FORCE
		);

		if (input->GetKeyDown(marioKeySet.Jump) && canHighJump && onAir)
		{
			jumpForce = MARIO_HIGH_JUMP_FORCE;
			if (maxRun && pMeter >= PMETER_MAX)
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
		if (rigidbody->GetVelocity().y > 2 * rigidbody->GetGravity() * Game::DeltaTime())
		{
			onGround = false;
			physicState.jump = JumpingStates::Fall;
		}
	}

	if (canCrouch) CrouchDetection(input);
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

void CMario::OnKeyDown(int keyCode)
{
	if (keyCode == marioKeySet.Jump && onGround && physicState.jump == JumpingStates::Stand)
	{
		rigidbody->SetVelocity(&Vector2(rigidbody->GetVelocity().x, -MARIO_JUMP_FORCE));
		physicState.jump = JumpingStates::Jump;

		onGround = false;
		canHighJump = true;
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
			collider->GetGameObject()->GetTag() == ObjectTags::GhostPlatform)
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
}

void CMario::MovementAnimation()
{
	auto animation = GetState(currentState);

	if (physicState.movement == MovingStates::Idle)
	{
		SetState("Idle");
		animation->SetSpeedMultiplier(1.0f);
	}

	if (physicState.movement == MovingStates::Walk || physicState.movement == MovingStates::Run)
	{
		auto speed = Mathf::Abs(rigidbody->GetVelocity().x);

		if (run)
			SetState("Run");
		else if (skid)
			SetState("Skid");
		else
			SetState("Walk");

		animation->SetSpeedMultiplier(Mathf::Clamp(speed / MARIO_WALK_SPEED, 1.0f, 4.0f));
	}

	if (physicState.movement == MovingStates::Crouch)
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

	if (pMeter >= PMETER_MAX)
	{
		SetState("Fly");
		return;
	}

	if (physicState.jump == JumpingStates::Jump || physicState.jump == JumpingStates::High)
	{
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
		Vector2 delta = MARIO_BBOX - MARIO_SMALL_BBOX;
		colliders->at(0)->SetLocalPosition(delta * 0.5f);
		physicState.movement = MovingStates::Crouch;
	}
	else
	{
		colliders->at(0)->SetBoxSize(MARIO_BBOX);
		colliders->at(0)->SetLocalPosition(VectorZero());
	}
}

bool CMario::IsStateTransition(MovingStates srcState, MovingStates dstState)
{
	return (prevPhysicState.movement == srcState && physicState.movement == dstState);
}
