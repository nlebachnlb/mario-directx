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

	void RegisterToScene(Scene* scene);
	void SwitchToState(std::string state);

	void ContinueSwitchingState();

private:
	void LinkStates();

	std::unordered_map<std::string, IState*> playerStates;
	std::unordered_map<std::string, CMario*> stateGameObjects;
	CMario* currentStateObject;

	int invincibleTime;
	bool invincible;
	bool waiting;

	std::string curState, targetState;
};

