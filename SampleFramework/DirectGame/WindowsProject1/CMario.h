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
#include "WarpUtils.h"
#include "WarpMark.h"

const int	MARIO_MIN_VDISTANCE			= 8; // pixels
const float MARIO_GRAVITY				= 0.0026f;
const float MARIO_FALL_LIMIT_VEL		= 0.85f;
const float MARIO_WALK_SPEED			= 0.25f;
const float MARIO_RUN_SPEED				= 0.65f;
const float MARIO_JUMP_FORCE			= 0.53f;

const float MARIO_MAX_JUMPHEIGHT		= 48 * 3.5f; // px
const float MARIO_MAX_SUPER_JUMPHEIGHT	= 48 * 4.5f; // px
const float MARIO_BUMP_FORCE			= 0.15f;

const int	PMETER_MAX					= 7;
const float PMETER_STEP					= 0.006f;

const float MARIO_WALK_ACCELERATION		= 0.000276f * 2;
const float MARIO_RUN_ACCELERATION		= 0.000286f * 2;
const float MARIO_WALK_DRAG_FORCE		= 0.0002506f * 3;
const float MARIO_RUN_DRAG_FORCE		= 0.0005266f * 2;
const float MARIO_SKID_ACCELERATION		= 0.001104f * 2;
const int	MARIO_FEVER_TIME			= 1500; // (miliseconds)

const int	MARIO_FLICK_DELTA			= 10; // miliseconds
const int	MARIO_INVINCIBLE_TIME		= 1300;

const Vector2 MARIO_BBOX(12 * 3, 26 * 3);
const Vector2 MARIO_SMALL_BBOX(12 * 3, 14 * 3);

const float MARIO_WARP_SPEED			= 0.12f;

struct MarioKeySet
{
	int Left, Right, Jump, LowJump, Attack, Crouch, HeadUp;
	// Default Keyset
	MarioKeySet()
	{
		Left	= DIK_LEFT;
		Right	= DIK_RIGHT;
		Jump	= DIK_S;
		Attack	= DIK_A;
		LowJump = DIK_X;
		Crouch	= DIK_DOWN;
		HeadUp	= DIK_UP;
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
	virtual void OnAnimationEnd() override;

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

	float GetPMeter();

	void PassPrivateData(CMario* other, bool moveData = true); 
	// if moveData, data of this will be reset

	void StartWarping(WarpDirection direction);
	void Warp();
	void WarpOut();
	void EndWarping();
	bool IsWarping();

	bool CanControl();
	void FinishLevel();

	void HoldVirtualKey(int keyCode);
	void ReleaseVirtualKey(int keyCode);
	void PressVirtualKeyDown(int keyCode);

	void Kick();

	MarioKeySet marioKeySet;

protected:
	virtual void InitAnimations();
	virtual void MovementAnimation();
	virtual void JumpingAnimation();
	virtual void WarpAnimation();
	void ResetPrivateData();
	bool canCrouch;
	MarioStateSet prevPhysicState, physicState;
	bool onGround;
	float pMeter;
	bool maxRun;
	bool runningRestriction; // If this flag is turned on, run state will be restricted
	bool canWarp;
	bool hold, kick;
	int warp;
	WarpDirection warpDirection;

	bool invincible;
	int flickTimer, visualAlpha;

	int feverTime; // The duration of PMeter keeping maximum value
	int feverState; // -1: disable fever mode (for raccoon, he has his own flying mechanic)
	int lastFeverTime;
	int facing, nx, pushSide;
	InputHandler* input;
	PlayerController* controller;
	Camera* mainCamera;

	std::unordered_map<int, bool> virtualKeyBinds;

private:
	void WarpProcess();
	void SkidDetection(Vector2 velocity);
	void CrouchDetection(InputHandler* input);
	void HoldProcess();
	bool IsStateTransition(MovingStates srcState, MovingStates dstState);

	void JumpState();
	void HighJumpState();
	void FallState();
	void StandState();
	void FeverProcess();

	bool skid, run;
	bool canHighJump, deflect, autoControl;

	float prevTargetVelocityX, targetVelocityX;
	float jumpTime;
	Vector2 previousVelocity, posBeforeJump;
	Holdable* heldInHandsObject;
	WarpMark* mark;
	GameObject standOnPlatform;
};
