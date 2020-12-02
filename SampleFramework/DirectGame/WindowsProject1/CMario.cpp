#include "CMario.h"
#include "Game.h"
#include "AnimationDatabase.h"
#include "Consts.h"
#include "Mathf.h"
#include "MarioCollider.h"
#include "QuestionBlock.h"
#include "KoopasShell.h"
#include "WarpMark.h"
#include "WarpEntrance.h"

void CMario::Awake()
{
	SetTag(ObjectTags::BigMario);
	DebugOut(L"Mario Awake\n");
	renderOrder = 1;

	InitAnimations();
	 
	MarioCollider* collider = new MarioCollider();
	collider->SetBoxSize(MARIO_BBOX);
	collider->AttachToEntity(this);
	collider->SetName("Normal");
	this->colliders->push_back(collider); 

	canCrouch = true;
	input = nullptr;
	mainCamera = nullptr;
}

void CMario::Start()
{
	SetState("Idle");

	this->rigidbody->SetGravity(MARIO_GRAVITY);
	this->rigidbody->SetDynamic(true);
	this->rigidbody->SetAcceleration(MARIO_WALK_ACCELERATION);
	this->rigidbody->SetDrag(Vector2(MARIO_WALK_DRAG_FORCE, 0.0f));
	this->rigidbody->SetFallLimitVelocity(MARIO_FALL_LIMIT_VEL);
	rigidbody->GetMaterial().AddBypassTag(ObjectTags::FriendlyProjectiles);
	rigidbody->GetMaterial().AddBypassTag(ObjectTags::BigMario);
	rigidbody->GetMaterial().AddBypassTag(ObjectTags::PowerupMario);
	rigidbody->GetMaterial().AddBypassTag(ObjectTags::SmallMario);

	physicState = { MovingStates::Idle, JumpingStates::Stand };
	facing = 1;
	skid = false;

	canHighJump = false;
	deflect = false;
	invincible = false;
	flickTimer = 0;

	pMeter = 0.0f;
	runningRestriction = false;

	feverState = 0;
	feverTime = MARIO_FEVER_TIME;
	lastFeverTime = 0;

	hold = false;
	heldInHandsObject = nullptr;

	warp = 0;
	canWarp = false;
}

void CMario::Update()
{
	if (input == nullptr) input = Game::GetInstance().GetService<InputHandler>();

	WarpProcess();
	if (warp != 0) return;

	auto velocity = rigidbody->GetVelocity();
	previousVelocity = velocity;
	prevPhysicState = physicState;
	prevTargetVelocityX = targetVelocityX;
	auto dt = Game::DeltaTime(); 

#pragma region Horizontal Movement
	auto curVelocity = velocity.x;
	if (pushSide == 0 && (input->GetKeyDown(marioKeySet.Left) || input->GetKeyDown(marioKeySet.Right)))
	{
		// Accelerate velocity based on moving states
		if (input->GetKeyDown(marioKeySet.Attack) && runningRestriction == false)
		{
			physicState.movement = MovingStates::Run;
			rigidbody->SetAcceleration(skid ? MARIO_SKID_ACCELERATION : MARIO_RUN_ACCELERATION);
			rigidbody->SetDrag(Vector2(MARIO_RUN_DRAG_FORCE, rigidbody->GetDrag().y));
		}
		else
		{
			physicState.movement = MovingStates::Walk;
			rigidbody->SetAcceleration(MARIO_WALK_ACCELERATION * (skid ? 2 : 1));
			rigidbody->SetDrag(Vector2(MARIO_WALK_DRAG_FORCE, rigidbody->GetDrag().y));
		}

		auto constSpeed = physicState.movement == MovingStates::Run ? MARIO_RUN_SPEED : MARIO_WALK_SPEED;
		targetVelocityX = constSpeed;
		
		if (input->GetKeyDown(marioKeySet.Left) && pushSide == 0)
			targetVelocityX = -1 * constSpeed, nx = -1;
		else if (input->GetKeyDown(marioKeySet.Right) && pushSide == 0)
			targetVelocityX = +1 * constSpeed, nx = +1;
		
		if (Mathf::Abs(curVelocity - targetVelocityX) > rigidbody->GetAcceleration() * dt)
		{
			if (curVelocity < targetVelocityX)
				curVelocity += rigidbody->GetAcceleration() * dt;
			else
				curVelocity -= rigidbody->GetAcceleration() * dt;
		}
		else
			curVelocity = targetVelocityX;

		velocity.x = curVelocity;
		facing = Mathf::Sign(velocity.x);

		SkidDetection(velocity);
	}
	else
	{
		// If Mario stops moving, the drag force will reduce the remaining speed
		if (Mathf::Abs(curVelocity) > rigidbody->GetDrag().x * dt) 
			curVelocity = Mathf::Abs(curVelocity) - (rigidbody->GetDrag().x * dt);
		else
		{
			curVelocity = 0.0f;

			// If Mario does not move anymore, he will be idle
			if (physicState.movement != MovingStates::Idle)
				physicState.movement = MovingStates::Idle;
		}

		velocity.x = facing * curVelocity;
	}

	run = Mathf::Abs(Mathf::Abs(velocity.x)) > MARIO_RUN_SPEED * 0.92f;
	maxRun = Mathf::Abs(Mathf::Abs(velocity.x)) > MARIO_RUN_SPEED * 0.95f;
	if (pushSide == 0) rigidbody->SetVelocity(&velocity);

#pragma endregion

	FeverProcess();

	// Keep Mario inside Camera bounds
	if (mainCamera == nullptr)
		mainCamera = Game::GetInstance().GetService<SceneManager>()->GetActiveScene()->GetMainCamera();
	transform.Position.x = Mathf::Clamp(transform.Position.x, mainCamera->GetPosition().x + MARIO_BBOX.x, mainCamera->GetPosition().x + mainCamera->GetViewportSize().x - MARIO_BBOX.x);

#pragma region Vertical Movement
	switch (physicState.jump)
	{
	case JumpingStates::Jump:
		JumpState();
		break;
	case JumpingStates::High:
		HighJumpState();
		break;
	case JumpingStates::Fall:
		FallState();
		break;
	case JumpingStates::Stand:
		StandState();
		break;
	}
#pragma endregion

	if (canCrouch && !hold) CrouchDetection(input);
	HoldProcess();
}

void CMario::LateUpdate()
{
}

void CMario::PreRender()
{
	if (invincible)
	{
		if (flickTimer > MARIO_FLICK_DELTA)
		{
			flickTimer = 0;
			visualAlpha = 255 - visualAlpha + 128;
		}

		flickTimer += Game::DeltaTime();
	}
	else
		visualAlpha = 255;

	if (!currentState.empty()) 
		animations.at(currentState)->SetAlpha(visualAlpha);

	if (warp)
		WarpAnimation();
	else
	{
		if (physicState.jump == JumpingStates::Stand)
			MovementAnimation();
		else
			JumpingAnimation();
	}

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
	posBeforeJump = transform.Position;

	onGround = false;
	canHighJump = true;
	this->deflect = deflect;
}

bool CMario::IsReadyToRun()
{
	return physicState.movement == MovingStates::Run;
}

void CMario::HoldObject(Holdable* holdableObj)
{
	if (holdableObj == nullptr)
	{
		hold = false;
		this->heldInHandsObject = nullptr;
		return;
	}

	hold = true;
	this->heldInHandsObject = holdableObj;
	holdableObj->PassToHolder(this);
}

void CMario::ReleaseInHandObject()
{
	hold = false;
	if (heldInHandsObject != nullptr) heldInHandsObject->Release();
	heldInHandsObject = nullptr;
}

Holdable* CMario::GetInHandObject()
{
	return this->heldInHandsObject;
}

void CMario::SetController(PlayerController* controller)
{
	this->controller = controller;
}

bool CMario::IsInvincible()
{
	return this->invincible;
}

void CMario::SetInvincible(bool invincible)
{
	this->invincible = invincible;
}

void CMario::PassPrivateData(CMario* other, bool moveData)
{
	other->posBeforeJump = posBeforeJump;
	other->physicState = physicState;
	other->pMeter = pMeter;
	other->feverState = feverState;
	other->feverTime = feverTime;
	other->lastFeverTime = lastFeverTime;
	//other->hold = hold;
	//other->heldInHandsObject = heldInHandsObject;
	
	other->pushSide = pushSide;
	other->invincible = invincible;
	// ResetPrivateData();
}

void CMario::StartWarping(WarpDirection direction)
{
	warp = 1;
	// rigidbody->SetDynamic(false);
	rigidbody->SetGravity(0);
	colliders->at(0)->SetTrigger(true);
	warpDirection = direction;
	renderOrder = -5;
}

void CMario::EndWarping()
{
	warp = 0;
	// rigidbody->SetDynamic(true);
	rigidbody->SetGravity(MARIO_GRAVITY);
	colliders->at(0)->SetTrigger(false);
	warpDirection = WarpDirection::None;
	renderOrder = 6;
}

bool CMario::IsWarping()
{
	return this->warp > 0;
}

#pragma region Keyboard

void CMario::OnKeyDown(int keyCode)
{
	if (pushSide != 0) return;
	if (keyCode == marioKeySet.Jump && onGround && physicState.jump == JumpingStates::Stand)
		Jump(MARIO_JUMP_FORCE * 0.3f);
}

void CMario::OnKeyUp(int keyCode)
{

}

#pragma endregion

#pragma region Collision

void CMario::OnCollisionEnter(Collider2D* selfCollider, vector<CollisionEvent*> collisions)
{
	for (auto collision : collisions)
	{
		auto collider = collision->collider;
		if (collider->GetGameObject()->GetTag() == ObjectTags::Solid ||
			collider->GetGameObject()->GetTag() == ObjectTags::GhostPlatform || 
			collider->GetGameObject()->GetTag() == ObjectTags::Block)
		{
			// DebugOut(L"Hit Solid: %f\n", collision->collisionDirection.y);
			if (collision->collisionDirection.y < 0 &&
				collision->collisionDirection.x <= 0.00001f)
			{
				onGround = true;
				physicState.jump = JumpingStates::Stand;
			}

			if (collision->collisionDirection.y > 0)
			{
				rigidbody->SetVelocity(&Vector2(rigidbody->GetVelocity().x, MARIO_BUMP_FORCE));
				if (physicState.jump == JumpingStates::Jump || physicState.jump == JumpingStates::High)
					physicState.jump = JumpingStates::Fall;

				if (collider->GetGameObject()->GetTag() == ObjectTags::Block)
					static_cast<AbstractBlock*>(collider->GetGameObject())->Bounce(this);
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

void CMario::OnOverlapped(Collider2D* self, Collider2D* other)
{
	auto otherTag = other->GetGameObject()->GetTag();

	if (otherTag == ObjectTags::WarpEntrance && warp == 0)
	{
		auto entrance = static_cast<WarpEntrance*>(other->GetGameObject());
		int warpKey;
		switch (entrance->GetWarpDirection())
		{
		case WarpDirection::Left: warpKey = marioKeySet.Left; break;
		case WarpDirection::Up: warpKey = marioKeySet.HeadUp; break;
		case WarpDirection::Right: warpKey = marioKeySet.Right; break;
		case WarpDirection::Down: warpKey = marioKeySet.Crouch; break;
		}

		if (input->GetKeyDown(warpKey) && 
			(warpKey == marioKeySet.HeadUp ? input->GetKeyDown(marioKeySet.Jump) : true))
		{
			StartWarping(entrance->GetWarpDirection());
			DebugOut(L"Enter warp: %d\n", warp);
		}
	}

	// Warp process
	if (warp == 1)
	{
		auto direction = WarpUtils::ToVector(warpDirection);
		auto dt = Game::DeltaTime() * Game::GetTimeScale();
		switch (otherTag)
		{
		case ObjectTags::WarpMark:
		{
			auto o = static_cast<WarpMark*>(other->GetGameObject());
			transform.Position = o->GetDestination();
			warpDirection = o->GetDirection();
			auto bset = mainCamera->GetBoundarySet(o->GetCameraBoundId());
			mainCamera->SetBoundary(bset.boundary);
			mainCamera->SetPosition(bset.position);
			warp = 2;
			DebugOut(L"switch: %d\n", warp);
		}
			break;
		}
		return;
	}

	if (TagUtils::PowerupTag(otherTag) || TagUtils::ItemTag(otherTag))
	{
		switch (otherTag)
		{
		case ObjectTags::RedMushroom:
			if (tag != ObjectTags::PowerupMario) controller->SwitchToState("BigMario");
			break;
		case ObjectTags::FireFlower:
			if (tag == ObjectTags::BigMario || tag == ObjectTags::PowerupMario) controller->SwitchToState("FireMario");
			else controller->SwitchToState("BigMario");
			break;
		case ObjectTags::RaccoonLeaf:
			if (tag == ObjectTags::BigMario || tag == ObjectTags::PowerupMario) controller->SwitchToState("RaccoonMario");
			else controller->SwitchToState("BigMario");
			break;
		case ObjectTags::Coin:
			// Increase coin data
			break;
		}

		Destroy(other->GetGameObject());
	}

	if (TagUtils::StaticTag(otherTag) && warp == 0)
	{
		// DebugOut(L"Enter overlap\n");
		auto selfBox = colliders->at(0)->GetBoundingBox();
		auto otherBox = other->GetBoundingBox();
		Vector2 headPoint(transform.Position.x, selfBox.top + 6);
		Vector2 footPoint(transform.Position.x, selfBox.bottom - 6);

		if (pushSide == 0)
		{
			std::vector<Collider2D*> headHits, footHits;
			auto raycast = Game::GetInstance().Raycast2D();
			bool goLeft = false, goRight = false;
			int i, leftLength, rightLength;
			float dist = 0, mDist;
			RectF m;

			// Shoot to left side
			dist = mDist = 0;
			raycast->HShoot(headPoint, HRayDirection::Left, ObjectTags::Block, 48 * 7, headHits);
			raycast->HShoot(footPoint, HRayDirection::Left, ObjectTags::Block, 48 * 7, footHits);

			headHits.insert(headHits.begin(), other);
			for (i = 0; i < headHits.size() - 1; ++i)
			{
				dist = headHits[i]->GetBoundingBox().left - headHits[i + 1]->GetBoundingBox().right;
				if (dist >= self->GetBoxSize().x) break;
			}

			if (footHits.size() == 0 || headHits.size() <= 1) goLeft = true;
			else
			{
				m = footHits[0]->GetBoundingBox();
				mDist = m.right - headHits[i]->GetBoundingBox().left;
				goLeft = mDist >= 0;
					//&& mDist <= dist;
			}

			DebugOut(L"LEFT: d=%f, m=%f\n", dist, mDist);
			leftLength = headHits.size();

			// Shoot to right side
			dist = mDist = 0; 
			raycast->HShoot(headPoint, HRayDirection::Right, ObjectTags::Block, 48 * 7, headHits);
			raycast->HShoot(footPoint, HRayDirection::Right, ObjectTags::Block, 48 * 7, footHits);

			headHits.insert(headHits.begin(), other);
			for (i = 0; i < headHits.size() - 1; ++i)
			{
				dist = headHits[i + 1]->GetBoundingBox().left - headHits[i]->GetBoundingBox().right;
				if (dist >= self->GetBoxSize().x) break;
			}

			if (footHits.size() == 0 || headHits.size() <= 1) DebugOut(L"Pass RIGHT\n"), goRight = true;
			else
			{
				m = footHits[0]->GetBoundingBox();
				mDist = m.left - headHits[i + 1]->GetBoundingBox().right;
				goRight = mDist >= 0;
					// && mDist <= dist;
			}

			rightLength = headHits.size();

			if (goLeft == goRight) pushSide = leftLength < rightLength ? -1 : 1;
			else pushSide = goLeft ? -1 : 1;

			/*DebugOut(L"Head: ");
			for (auto h : headHits) DebugOut(L"- (%f, %f) - ", h->GetBoundingBox().left, h->GetBoundingBox().right);
			DebugOut(L"\n");

			DebugOut(L"RIGHT: d=%f, m=%f, size=%d\n", dist, mDist, headHits.size());
			DebugOut(L"Go left:%d, go right: %d, side: %d\n", goLeft ? 1 : 0, goRight ? 1 : 0, pushSide);*/
		}
		
		transform.Position.x += pushSide * 0.15f * Game::DeltaTime();
	}

	if (TagUtils::EnemyTag(otherTag) && other->IsTrigger() && warp == 0)
	{
		if (!IsInvincible())
		{
			auto enemy = static_cast<AbstractEnemy*>(other->GetGameObject());
			OnDamaged(enemy);
		}
	}

	if (otherTag == ObjectTags::HostileProjectiles && !IsInvincible() && warp == 0)
		OnDamaged(nullptr);
}

void CMario::OnSolidOverlappedExit()
{
	pushSide = 0;

	if (warp == 2)
	{
		DebugOut(L"warp = 2, end\n");
		EndWarping();
	}
}

void CMario::OnDamaged(AbstractEnemy* enemy)
{
	if (invincible) return;

	// DebugOut(L"[Mario] Damaged\n");
	invincible = true;
	switch (tag)
	{
	case ObjectTags::SmallMario:
		// Dead event here
		break;
	case ObjectTags::BigMario:
		controller->SwitchToState("SmallMario");
		break;
	case ObjectTags::PowerupMario:
		controller->SwitchToState("BigMario");
		break;
	}
}

#pragma endregion

#pragma region Animation

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
	AddAnimation("HoldJump",	animations->Get("ani-big-mario-hold-jump"));
	AddAnimation("Kick",		animations->Get("ani-big-mario-kick"));

	AddAnimation("WarpHor", animations->Get("ani-big-mario-walk"));
	AddAnimation("WarpVer", animations->Get("ani-big-mario-idle-front"));
}

void CMario::MovementAnimation()
{
	auto animation = GetState(currentState);

	if (physicState.movement == MovingStates::Idle)
	{
		SetState(hold ? "HoldIdle" : "Idle");
		animation->SetSpeedMultiplier(1.0f);
	}
	else if (physicState.movement == MovingStates::Walk || physicState.movement == MovingStates::Run)
	{
		auto speed = Mathf::Abs(rigidbody->GetVelocity().x);

		if (run)
			SetState(hold ? "HoldMove" : "Run");
		else if (skid && !hold)
			SetState("Skid");
		else
			SetState(hold ? "HoldMove" : "Walk");

		animation->SetSpeedMultiplier(Mathf::Clamp(speed / MARIO_WALK_SPEED, 1.0f, 4.0f));
	}
	else if (physicState.movement == MovingStates::Crouch && !hold)
		SetState("Crouch");

	SetVisualRelativePosition(colliders->at(0)->GetLocalPosition());
}

void CMario::JumpingAnimation()
{
	if (physicState.movement == MovingStates::Crouch) return;
	if (currentState.compare("Fly") == 0) return;

	if (feverState == 2 && !hold)
	{
		SetState("Fly");
		return;
	}

	if (physicState.jump == JumpingStates::Jump || physicState.jump == JumpingStates::High)
		SetState(hold ? "HoldJump" : "Jump");
	else if (physicState.jump == JumpingStates::Fall)
		SetState(hold ? "HoldJump" : "Fall");
}

void CMario::WarpAnimation()
{
	auto vecWDir = WarpUtils::ToVector(warpDirection);
	if (vecWDir.x != 0 && currentState.compare("WarpHor") != 0) SetState("WarpHor");
	if (vecWDir.y != 0 && currentState.compare("WarpVer") != 0) SetState("WarpVer");
}

void CMario::ResetPrivateData()
{
	hold = false;
	heldInHandsObject = nullptr;
}

#pragma endregion

#pragma region Skid-Crouch-Hold

void CMario::WarpProcess()
{
	if (warp > 0)
	{
		auto dt = Game::DeltaTime() * Game::GetTimeScale();
		rigidbody->SetVelocity(&Vector2(0, 0));
		auto direction = WarpUtils::ToVector(warpDirection);
		transform.Position += direction * MARIO_WARP_SPEED * dt;
		// DebugOut(L"Warp: %f, %f, %f, %f, %f\n", direction.x, direction.y, rigidbody->GetVelocity().x, rigidbody->GetVelocity().y, rigidbody->GetGravity());
	}
}

void CMario::SkidDetection(Vector2 velocity)
{
	skid = Mathf::Sign(velocity.x) * nx < 0;
}

void CMario::CrouchDetection(InputHandler* input)
{
	if (warp > 0) return;

	bool exitConditions = 
	physicState.jump == JumpingStates::Stand && 
		(input->GetKeyDown(marioKeySet.Left) || input->GetKeyDown(marioKeySet.Right));

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
	if (heldInHandsObject == nullptr)
	{
		hold = false;
		return;
	}

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
			ReleaseInHandObject();
		}
	}
}

#pragma endregion

bool CMario::IsStateTransition(MovingStates srcState, MovingStates dstState)
{
	return (prevPhysicState.movement == srcState && physicState.movement == dstState);
}

#pragma region Jump States Processor

void CMario::JumpState()
{
	auto velocity = rigidbody->GetVelocity();
	auto jumpForce = MARIO_JUMP_FORCE;
	canHighJump = input->GetKeyDown(marioKeySet.Jump);

	if (canHighJump)
	{
		auto maxHeight = ((feverState == 2 && run) || deflect) ? MARIO_MAX_SUPER_JUMPHEIGHT : MARIO_MAX_JUMPHEIGHT;
		if (Mathf::Abs(posBeforeJump.y - transform.Position.y) <= maxHeight)
		{
			velocity.y = -MARIO_JUMP_FORCE;
			rigidbody->SetVelocity(&velocity);
		}
		else
		{
			velocity.y = -MARIO_JUMP_FORCE * 0.5f;
			rigidbody->SetVelocity(&velocity);
			physicState.jump = JumpingStates::High;
		}
	}
	else
		physicState.jump = JumpingStates::High;
}

void CMario::HighJumpState()
{
	auto velocity = rigidbody->GetVelocity();
	rigidbody->SetGravity(MARIO_GRAVITY);

	// After reaching the max jump distance, fall down
	if (velocity.y > 0)
	{
		physicState.jump = JumpingStates::Fall;
	}
}

void CMario::FallState()
{
	// Fall and land on ground, switch to stand
	if (onGround)
		physicState.jump = JumpingStates::Stand;
}

void CMario::StandState()
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

void CMario::FeverProcess()
{
	auto velocity = rigidbody->GetVelocity();
	// If Mario runs at max curVelocity, the P Meter starts increasing
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
		pMeter = Mathf::Clamp(pMeter - PMETER_STEP * 2.0f * Game::DeltaTime(), 0.0, PMETER_MAX);

	if (feverState == 2)
	{
		pMeter = PMETER_MAX;
		if (GetTickCount() - lastFeverTime > feverTime || physicState.movement != MovingStates::Run)
			feverState = 0;
	}
}

#pragma endregion