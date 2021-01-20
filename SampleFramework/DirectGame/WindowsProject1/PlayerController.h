#pragma once
#include "CMario.h"
#include "StateMachine.h"
#include "Scene.h"

class CMario;
class PlayerController : public CGameObject, public StateMachine
{
public:
	void Awake()	override;
	void Start()	override;
	void Update()	override;
	void OnKeyDown(int keyCode) override;

	void InitStates();
	void RegisterToScene(Scene* scene);
	void SwitchToState(std::string state);

	void ContinueSwitchingState();

	CMario* GetMario();

	void SetActing(bool value);
	bool IsActing();

	void ActingControl();

private:
	void LinkStates();

	std::unordered_map<std::string, IState*> playerStates;
	std::unordered_map<std::string, CMario*> stateGameObjects;
	CMario* currentStateObject;
	InputHandler* input;

	int invincibleTime;
	bool invincible;
	bool waiting;
	bool acting = false;
	int actingPhase;

	std::string curState, targetState;
};

