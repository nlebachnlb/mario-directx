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
	controllable = true;
}

void MarioLocator::Start()
{
	auto data = Game::GetInstance().GetData();
	SetState(data->GetPowerup());
	currentNode = data->GetWorldMapTempData().currentNodeID;
	onGoing = 0;
}

void MarioLocator::Update()
{
	if (map->GetNode(currentNode) == nullptr) return;

	auto dt = Game::DeltaTime() * Game::GetTimeScale();

	switch (recover)
	{
	case 1:
	{
		timer += dt;
		if (timer > 500)
		{
			recover = 2;
			timer = 0;
			onGoing = 1;
		}
	}
	break;
	case 2:
	{
		if (onGoing == 0)
		{
			transform->Scale = VectorZero();
			timer = 0;
			recover = 3;
		}
	}
	break;
	case 3:
	{
		timer += dt;
		if (timer > 500)
		{
			timer = 0;
			recover = 0;
			transform->Scale = Vector2(1, 1);
		}
	}
	break;
	}

	auto delta = destination - transform->Position;
	if (Mathf::Magnitude(delta) > MOVE_SPEED * dt && onGoing)
	{ 
		auto normalized = Mathf::Normalize(delta);
		transform->Position = transform->Position + normalized * MOVE_SPEED * dt;
	}
	else if (onGoing == 1)
	{
		onGoing = 0;
		transform->Position = destination;
	}
}

void MarioLocator::OnKeyDown(int keyCode)
{
	if (map->GetNode(currentNode) == nullptr) return;
	auto canvas = static_cast<MainCanvas*>(Canvas::GetCanvas("main"));
	if (canvas->IsDialogOpening()) return;

	if (onGoing || recover) return;

	auto node = map->GetNode(currentNode);
	if (keyCode == DIK_A)
	{
		if (Game::GetInstance().GetSourcePathOf(CATEGORY_SCENE, node->GetSceneID()).empty())
			return;

		auto data = Game::GetInstance().GetData();
		auto temp = data->GetWorldMapTempData();
		temp.currentNodeID = currentNode;
		data->SetWorldMapTempData(temp);

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
	DebugOut(L"NODE: %d\n", currentNode);
	auto node = map->GetNode(id);
	if (node == nullptr) return;
	transform->Position = node->GetPosition();

	auto tempData = Game::GetInstance().GetData()->GetWorldMapTempData();
	if (tempData.status != GameplayStatus::None)
	{
		switch (tempData.status)
		{
		case GameplayStatus::Lose:
		{
			auto lastNode = map->GetNode(tempData.lastNodeID);
			destination = lastNode ? lastNode->GetPosition() : node->GetPosition();
			currentNode = tempData.currentNodeID = tempData.lastNodeID;
			recover = 1;
		}
		break;
		case GameplayStatus::Victory:
		{
			tempData.lastNodeID = tempData.currentNodeID;
		}
		break;
		}

		tempData.status = GameplayStatus::None;
	}
}

void MarioLocator::SetControllable(bool value)
{
	controllable = value;
}

bool MarioLocator::IsControllable()
{
	return controllable && !(onGoing || recover);
}

Graph* MarioLocator::Map()
{
	return map;
}
