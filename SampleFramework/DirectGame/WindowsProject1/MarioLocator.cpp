#include "MarioLocator.h"
#include "Game.h"
#include "AnimationDatabase.h"
#include "Mathf.h"
#include "MainCanvas.h"

void MarioLocator::Awake()
{
	InitAnimations();
	map = new Graph();
	renderOrder = 6;
}

void MarioLocator::Start()
{
	auto data = Game::GetInstance().GetData();
	SetState(data->GetPowerup());
	currentNode = 0;
	onGoing = 0;
}

void MarioLocator::Update()
{
	if (map->GetNode(currentNode) == nullptr) return;

	auto dt = Game::DeltaTime() * Game::GetTimeScale();
	auto delta = destination - transform.Position;
	if (Mathf::Magnitude(delta) > MOVE_SPEED * dt && onGoing)
	{ 
		auto normalized = Mathf::Normalize(delta);
		transform.Position = transform.Position + normalized * MOVE_SPEED * dt;
	}
	else if (onGoing == 1)
	{
		onGoing = 0;
		transform.Position = destination;
	}
}

void MarioLocator::OnKeyDown(int keyCode)
{
	if (map->GetNode(currentNode) == nullptr) return;
	if (onGoing) return;

	auto node = map->GetNode(currentNode);
	if (keyCode == DIK_A)
	{
		if (Game::GetInstance().GetSourcePathOf(CATEGORY_SCENE, node->GetSceneID()).empty())
			return;

		MainCanvas* canvas = static_cast<MainCanvas*>(Canvas::GetCanvas("main"));
		canvas->SetTargetScene(node->GetSceneID());
		canvas->GetGameReady();
		canvas->StartTransition();
		onGoing = 2;
		return;
	}

	int acceptedKey;
	for (auto adj : *node->GetAdjacentList())
	{
		switch (adj.weight)
		{
		case Weight::Left: acceptedKey = (DIK_LEFT); break;
		case Weight::Right: acceptedKey = (DIK_RIGHT); break;
		case Weight::Up: acceptedKey = (DIK_UP); break;
		case Weight::Down: acceptedKey = (DIK_DOWN); break;
		}

		if (keyCode == acceptedKey)
		{
			auto adjNode = map->GetNode(adj.nodeID);
			if (adjNode)
			{
				destination = adjNode->GetPosition();
				currentNode = adj.nodeID;
				onGoing = 1;
			}
			break;
		}
	}
}

void MarioLocator::InitAnimations()
{
	auto animations = Game::GetInstance().GetService<AnimationDatabase>();
	AddAnimation("SmallMario", animations->Get("ani-small-mario-map"));
	AddAnimation("BigMario", animations->Get("ani-big-mario-map"));
	AddAnimation("RaccoonMario", animations->Get("ani-raccoon-mario-map"));
	AddAnimation("FireMario", animations->Get("ani-fire-mario-map"));
}

void MarioLocator::SetMap(Graph* m)
{
	map = m;
}

void MarioLocator::SetCurrentNode(int id)
{
	currentNode = id;
	auto node = map->GetNode(id);
	if (node == nullptr) return;
	transform.Position = node->GetPosition();
}

Graph* MarioLocator::Map()
{
	return map;
}
