#include "PlayerController.h"
#include "Game.h"
#include "SmallMario.h"
#include "FireMario.h"
#include "RaccoonMario.h"
#include "EffectPool.h"
#include "MarioFX.h"
#include "ExampleScene.h"
#include "WorldMapScene.h"
#include "MainCanvas.h"

void PlayerController::Awake()
{
	global = true;
	actingPhase = 0;

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
	LinkStates();
	SwitchToState(acting ? "BigMario" : Game::GetInstance().GetData()->GetPowerup());
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

	if (acting) ActingControl();
}

void PlayerController::OnKeyDown(int keyCode)
{
	if (keyCode == DIK_RETURN)
		Game::GetInstance().Pause();

	if (keyCode == DIK_1)
		SwitchToState("SmallMario");
	else if (keyCode == DIK_2)
		SwitchToState("BigMario");
	else if (keyCode == DIK_3)
		SwitchToState("FireMario");
	else if (keyCode == DIK_4)
		SwitchToState("RaccoonMario");
	else if (keyCode == DIK_SPACE)
	{
		Game::GetInstance().GetService<SceneManager>()->LoadScene(new WorldMapScene());
	}
}

void PlayerController::InitStates()
{
	for (auto x : stateGameObjects)
	{
		auto object = x.second;
		object->SetPosition(this->transform->Position);
	}
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
	if (waiting) return;

	if (state.compare("Die") == 0)
	{
		auto canvas = static_cast<MainCanvas*>(Canvas::GetCanvas("main"));
		auto gmap = Game::GetInstance().GetService<GameMap>();
		auto fxPool = gmap->GetSpawnerManager()->GetService<EffectPool>();

		waiting = true;
		SwitchState(nullptr);

		Game::SetTimeScale(0);
		auto fx = fxPool->CreateFX("fx-mario-die", transform->Position);
		auto castFx = static_cast<MarioFX*>(fx);
		castFx->SetController(this);
		canvas->LoseGame();
		if (!acting) Game::GetInstance().GetData()->SetPowerup("SmallMario");
		return;
	}

	targetState = state;
	if (!acting) Game::GetInstance().GetData()->SetPowerup(state);

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

	if (curState.compare(targetState) == 0)
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
		transform->Position - (grow ? 0.5f * delta : VectorZero())
	);
	
	Game::SetTimeScale(0);

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

		stateGameObjects.at(state)->PassPrivateData(currentStateObject);

		// if (currentStateObject->GetInHandObject() != nullptr)
		stateGameObjects.at(state)->HoldObject(currentStateObject->GetInHandObject());
	}

	bool shrink = false;
	if (currentStateObject != nullptr && stateGameObjects.at(state) != nullptr)
		shrink = (int)currentStateObject->GetTag() > (int)stateGameObjects.at(state)->GetTag();
	currentStateObject = stateGameObjects.at(state);
	curState = targetState;
	
	if (shrink)
	{
		invincible = true;
		invincibleTime = 0;
		currentStateObject->SetInvincible(true);
	}

	SwitchState(playerStates.at(state));
}

CMario* PlayerController::GetMario()
{
	return this->currentStateObject;
}

void PlayerController::SetActing(bool value)
{
	acting = true;
}

bool PlayerController::IsActing()
{
	return acting;
}

void PlayerController::ActingControl()
{
	auto mario = GetMario();
}

void PlayerController::LinkStates()
{
	for (auto go = stateGameObjects.begin(); go != stateGameObjects.end(); go++)
	{
		auto obj = (*go).second;
		obj->SetController(this);
	}
}
