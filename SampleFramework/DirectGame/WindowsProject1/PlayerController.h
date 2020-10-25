#pragma once
#include "CMario.h"
#include "StateMachine.h"
#include "Scene.h"

class PlayerController : public CGameObject, public StateMachine
{
public:
	void Awake()	override;
	void Start()	override;
	void Update()	override;
	void OnKeyDown(int keyCode) override;

	void RegisterToScene(Scene* scene);

private:
	void SwitchToState(std::string state);
	std::unordered_map<std::string, IState*> playerStates;
	std::unordered_map<std::string, CMario*> stateGameObjects;
	CMario* currentStateObject;
};

