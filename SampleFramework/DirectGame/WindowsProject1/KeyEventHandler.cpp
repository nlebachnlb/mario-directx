#include "KeyEventHandler.h"
#include "Game.h"

void CGlobalKeyEventHandler::KeyState(BYTE* state)
{
}

void CGlobalKeyEventHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"Keydown\n");
	auto sceneManager = Game::GetInstance().GetService<SceneManager>();
	Scene* activeScene = sceneManager->GetActiveScene();
	auto gameObjects = *activeScene->GetSceneObjects();

	for (auto go : gameObjects)
	{
		if (go->IsEnabled())
			go->OnKeyDown(KeyCode);
	}
}

void CGlobalKeyEventHandler::OnKeyUp(int KeyCode)
{
	auto sceneManager = Game::GetInstance().GetService<SceneManager>();
	Scene* activeScene = sceneManager->GetActiveScene();
	auto gameObjects = *activeScene->GetSceneObjects();

	for (auto go : gameObjects)
	{
		if (go->IsEnabled())
			go->OnKeyUp(KeyCode);
	}
}
