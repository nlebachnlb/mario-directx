#include "PlayerController.h"
#include "Game.h"
#include "SmallMario.h"
#include "FireMario.h"
#include "RaccoonMario.h"
#include "EffectPool.h"
#include "MarioFX.h"

void PlayerController::Awake()
{
	SetTag(ObjectTags::Player);
	rigidbody->SetDynamic(false);
	auto activeScene = Game::GetInstance().GetService<SceneManager>()->GetActiveScene();

	CMario* obj;

	obj = Instantiate<CMario>();
	playerStates.insert(make_pair("BigMario", obj));
	stateGameObjects.insert(make_pair("BigMario", obj));
	obj->SetActive(false);

	obj = Instantiate<SmallMario>();
	playerStates.insert(make_pair("SmallMario", obj));
	stateGameObjects.insert(make_pair("SmallMario", obj));
	obj->SetActive(false);

	FireMario* fmObj = Instantiate<FireMario>();
	playerStates.insert(make_pair("FireMario", fmObj));
	stateGameObjects.insert(make_pair("FireMario", fmObj));
	fmObj->SetActive(false);

	obj = Instantiate<RaccoonMario>();
	playerStates.insert(make_pair("RaccoonMario", obj));
	stateGameObjects.insert(make_pair("RaccoonMario", obj));
	obj->SetActive(false);
}

void PlayerController::Start()
{
	for (auto x : stateGameObjects)
	{
		auto object = x.second;
		object->SetPosition(this->transform.Position);
	}

	LinkStates();
	SwitchToState("BigMario");
	waiting = false;
}

void PlayerController::Update()
{
	Execute();
	if (waiting) return;
	if (currentStateObject == nullptr) return;

	if (invincible)
	{
		invincibleTime += Game::DeltaTime();
		if (invincibleTime > MARIO_INVINCIBLE_TIME)
		{
			invincible = false, invincibleTime = 0;
			currentStateObject->SetInvincible(false);
		}
	}

	SetPosition(currentStateObject->GetTransform().Position);
	
	for (auto go = stateGameObjects.begin(); go != stateGameObjects.end(); go++)
	{
		auto obj = (*go).second;
		if (obj != currentStateObject)
		{
			Vector2 delta = VectorZero();
			if (currentStateObject->GetTag() == ObjectTags::SmallMario || 
				obj->GetTag() == ObjectTags::SmallMario)
				delta = MARIO_BBOX - MARIO_SMALL_BBOX;

			if (obj->GetTag() == ObjectTags::SmallMario)
				obj->SetPosition(currentStateObject->GetTransform().Position + delta * 0.5f);
			else 
				obj->SetPosition(currentStateObject->GetTransform().Position - delta * 0.5f);
		}
	}
}

void PlayerController::OnKeyDown(int keyCode)
{
	if (keyCode == DIK_1)
		SwitchToState("SmallMario");
	else if (keyCode == DIK_2)
		SwitchToState("BigMario");
	else if (keyCode == DIK_3)
		SwitchToState("FireMario");
	else if (keyCode == DIK_4)
		SwitchToState("RaccoonMario");
}

void PlayerController::RegisterToScene(Scene* scene)
{
	for (auto x : stateGameObjects)
		scene->AddObject(x.second);
	dynamic_cast<FireMario*>(stateGameObjects.at("FireMario"))->GetFireballs().RegisterPoolToScene(scene);
	dynamic_cast<RaccoonMario*>(stateGameObjects.at("RaccoonMario"))->RegisterToScene(scene);
}

void PlayerController::SwitchToState(std::string state)
{
	targetState = state;

	auto gmap = Game::GetInstance().GetService<GameMap>();
	if (gmap == nullptr)
	{
		ContinueSwitchingState();
		return;
	}

	auto spawner = gmap->GetSpawnerManager();
	if (spawner == nullptr)
	{
		ContinueSwitchingState();
		return;
	}

	auto grow	= curState.compare("SmallMario") == 0 && targetState.compare("BigMario") == 0;
	auto shrink = curState.compare("BigMario") == 0 && targetState.compare("SmallMario") == 0;
	auto fxName = grow ? "fx-mario-grow-up" : (shrink ? "fx-mario-shrink-down" : "fx-mario-transformation");

	auto delta = MARIO_BBOX - MARIO_SMALL_BBOX;
	auto fx = spawner->GetService<EffectPool>()->CreateFX(
		fxName, 
		transform.Position - (grow ? 0.5f * delta : VectorZero())
	);
	waiting = true;
	SwitchState(nullptr);
	if (fx == nullptr) 
		ContinueSwitchingState();
	else
	{
		auto castFx = static_cast<MarioFX*>(fx);
		castFx->SetController(this);

		if (currentStateObject != nullptr)
			castFx->SetScale(currentStateObject->GetTransform().Scale);
	}
}

void PlayerController::ContinueSwitchingState()
{
	auto state = targetState;
	waiting = false;

	if (currentStateObject != nullptr)
	{
		stateGameObjects.at(state)->SetFacing(currentStateObject->GetFacing());
		stateGameObjects.at(state)->GetRigidbody()->SetVelocity(&currentStateObject->GetRigidbody()->GetVelocity());
		stateGameObjects.at(state)->GetRigidbody()->SetGravity(currentStateObject->GetRigidbody()->GetGravity());
		stateGameObjects.at(state)->GetRigidbody()->SetAcceleration(currentStateObject->GetRigidbody()->GetAcceleration());
	}

	currentStateObject = stateGameObjects.at(state);
	curState = targetState;
	invincible = true;
	invincibleTime = 0;
	currentStateObject->SetInvincible(true);
	SwitchState(playerStates.at(state));
}

void PlayerController::LinkStates()
{
	for (auto go = stateGameObjects.begin(); go != stateGameObjects.end(); go++)
	{
		auto obj = (*go).second;
		obj->SetController(this);
	}
}
