#pragma once
#include "GameObject.h"
#include "Transform.h"
#include <dinput.h>
#include "InputHandler.h"
#include "IState.h"
#include "Holdable.h"
#include "Camera.h"
#include "PlayerController.h"
#include "AbstractEnemy.h"

const int	MARIO_MIN_VDISTANCE			= 8; // pixels
const float MARIO_GRAVITY				= 0.0026f;
const float MARIO_FALL_LIMIT_VEL		= 1.30f;
const float MARIO_WALK_SPEED			= 0.25f;
const float MARIO_RUN_SPEED				= 0.65f;
const float MARIO_JUMP_FORCE			= 0.53f;

const float MARIO_MAX_JUMPHEIGHT		= 48 * 4; // px
const float MARIO_MAX_SUPER_JUMPHEIGHT	= 48 * 5; // px
const float MARIO_BUMP_FORCE			= 0.15f;

const int	PMETER_MAX					= 7;
const float PMETER_STEP					= 0.006f;

const float MARIO_WALK_ACCELERATION		= 0.000376f * 2;
const float MARIO_RUN_ACCELERATION		= 0.000288f * 2;
const float MARIO_WALK_DRAG_FORCE		= 0.0002506f * 3;
const float MARIO_RUN_DRAG_FORCE		= 0.0006266f * 2;
const float MARIO_SKID_ACCELERATION		= 0.001104f * 2;
const int	MARIO_FEVER_TIME			= 1500; // (miliseconds)

const int	MARIO_FLICK_DELTA			= 5; // miliseconds
const int	MARIO_INVINCIBLE_TIME		= 2000;

const Vector2 MARIO_BBOX(12 * 3, 26 * 3);
const Vector2 MARIO_SMALL_BBOX(12 * 3, 14 * 3);

struct MarioKeySet
{
	int Left, Right, Jump, LowJump, Attack, Crouch;
	// Default Keyset
	MarioKeySet()
	{
		Left	= DIK_LEFT;
		Right	= DIK_RIGHT;
		Jump	= DIK_S;
		Attack	= DIK_A;
		LowJump = DIK_X;
		Crouch	= DIK_DOWN;
	}
};

enum class MovingStates
{ 
	Idle,
	Walk, 
	Run,
	Crouch,
	Attack
};

enum class JumpingStates
{
	Stand,
	Jump,
	High,
	Fall
};

struct MarioStateSet
{
	MovingStates movement;
	JumpingStates jump;
};

class PlayerController;
class CMario : public CGameObject, public IState
{
public:
	virtual void Awake()		override;
	virtual void Start()		override;
	virtual void Update()		override;
	virtual void LateUpdate()	override;
	virtual void PreRender()	override;
	virtual void OnKeyDown(int keyCode);
	virtual void OnKeyUp(int keyCode);
	virtual void OnCollisionEnter(Collider2D* selfCollider, vector<CollisionEvent*> collisions);
	virtual void OnTriggerEnter(Collider2D* selfCollider, vector<CollisionEvent*> collisions);
	virtual void OnOverlapped(Collider2D*, Collider2D*) override;
	virtual void OnSolidOverlappedExit() override;
	virtual void OnDamaged(AbstractEnemy* enemy);

	// State Interface implementation
	virtual void Entrance()		override;
	virtual void Execute()		override;
	virtual void Exit()			override;

	int GetFacing();
	int SetFacing(int facing);

	void Jump(float force = MARIO_JUMP_FORCE, bool deflect = false);
	bool IsReadyToRun();

	void HoldObject(Holdable* holdableObj);
	void ReleaseInHandObject();
	Holdable* GetInHandObject();
	void SetController(PlayerController* controller);

	bool IsInvincible();
	void SetInvincible(bool invincible);

	void PassPrivateData(CMario* other, bool moveData = true); 
	// if moveData, data of this will be reset

protected:
	virtual void InitAnimations();
	virtual void MovementAnimation();
	virtual void JumpingAnimation();
	void ResetPrivateData();
	bool canCrouch;
	MarioKeySet marioKeySet;
	MarioStateSet prevPhysicState, physicState;
	bool onGround;
	float pMeter;
	bool maxRun;
	bool runningRestriction; // If this flag is turned on, run state will be restricted

	bool invincible;
	int flickTimer, visualAlpha;

	int feverTime; // The duration of PMeter keeping maximum value
	int feverState; // -1: disable fever mode (for raccoon, he has his own flying mechanic)
	int lastFeverTime;
	int facing, nx, pushSide;
	InputHandler* input;
	PlayerController* controller;

private:
	void SkidDetection(Vector2 velocity);
	void CrouchDetection(InputHandler* input);
	void HoldProcess();
	bool IsStateTransition(MovingStates srcState, MovingStates dstState);

	void JumpState();
	void HighJumpState();
	void FallState();
	void StandState();
	void FeverProcess();

	bool animationKick;
	bool skid, run, hold;
	bool canHighJump, deflect;

	float prevTargetVelocityX, targetVelocityX;
	float jumpTime;
	Vector2 previousVelocity, posBeforeJump;
	Holdable* heldInHandsObject;
	Camera* mainCamera;
};
