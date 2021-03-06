#pragma once
#include "GameObject.h"
#include "Graph.h"

const float MOVE_SPEED = 0.45f;

class MarioLocator : public CGameObject
{
public:
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnKeyDown(int keyCode) override;

	void InitAnimations();

	void SetMap(Graph* m);
	void SetCurrentNode(int id);

	void SetControllable(bool value);
	bool IsControllable();

	Graph* Map();
private:
	Graph* map;
	int currentNode;
	Vector2 destination;
	int onGoing;
	int recover, timer;
	bool controllable;
};