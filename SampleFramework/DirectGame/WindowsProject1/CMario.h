#pragma once
#include "GameObject.h"
#include "Transform.h"
#include <dinput.h>
#include "InputHandler.h"
#include "IState.h"
#include "Holdable.h"

const int	MARIO_MIN_VDISTANCE			= 8; // pixels
const float MARIO_GRAVITY				= 0.00093f;
const float MARIO_WALK_SPEED			= 0.15f;
const float MARIO_RUN_SPEED				= 0.40f;
const float MARIO_JUMP_FORCE			= 0.25f;
const float MARIO_HIGH_JUMP_FORCE		= 0.53f;
const float MARIO_SUPER_JUMP_FORCE		= 0.63f;
const float MARIO_PUSH_FORCE			= 0.00093f;
const int	PMETER_MAX					= 7;
const float PMETER_STEP					= 0.003f;
// const float MARIO_ACCELERATION = 0.0276;
const float MARIO_WALK_ACCELERATION		= 0.000376f;
const float MARIO_RUN_ACCELERATION		= 0.000188f;
const float MARIO_WALK_DRAG_FORCE		= 0.0002506f;
const float MARIO_RUN_DRAG_FORCE		= 0.0006266f;
const float MARIO_SKID_ACCELERATION		= 0.001104f;
const int	MARIO_FEVER_TIME			= 1500; // (miliseconds)

const Vector2 MARIO_BBOX(12 * 3, 26 * 3);
const Vector2 MARIO_SMALL_BBOX(14 * 3, 14 * 3);

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

class CMario : public CGameObject, public IState
{
public:
	virtual void Awake()		override;
	virtual void Start()		override;
	virtual void Update()		override;
	virtual void PreRender()	override;
	virtual void OnKeyDown(int keyCode);
	virtual void OnKeyUp(int keyCode);
	virtual void OnCollisionEnter(Collider2D* selfCollider, vector<CollisionEvent*> collisions);
	virtual void OnTriggerEnter(Collider2D* selfCollider, vector<CollisionEvent*> collisions);

	// State Interface implementation
	virtual void Entrance()		override;
	virtual void Execute()		override;
	virtual void Exit()			override;

	int GetFacing();
	int SetFacing(int facing);

	void Jump(float force = MARIO_JUMP_FORCE, bool deflect = false);
	bool IsReadyToRun();

	void HoldObject(Holdable* holdableObj);

protected:
	virtual void InitAnimations();
	virtual void MovementAnimation();
	virtual void JumpingAnimation();
	bool canCrouch;
	MarioKeySet marioKeySet;
	MarioStateSet prevPhysicState, physicState;
	bool onGround;
	float pMeter;
	bool maxRun;
	bool runningRestriction; // If this flag is turned on, run state will be restricted
	int feverTime; // The duration of PMeter keeping maximum value
	int feverState; // -1: disable fever mode (for raccoon, he has his own flying mechanic)
	int lastFeverTime;
	int facing;
	InputHandler* input;

private:
	void SkidDetection(Vector2 velocity);
	void CrouchDetection(InputHandler* input);
	void HoldProcess();
	bool IsStateTransition(MovingStates srcState, MovingStates dstState);

	bool animationKick;
	bool skid, canSkid; 
	bool run, hold;
	bool highJump, canHighJump, deflect;
	float prevTargetVelocityX, targetVelocityX;
	Vector2 previousVelocity;
	Holdable* heldInHandsObject;
};
