#include "KeyEventHandler.h"
#include "Game.h"
#include "Canvas.h"

void CGlobalKeyEventHandler::KeyState(BYTE* state)
{
}

void CGlobalKeyEventHandler::OnKeyDown(int KeyCode)
{
	Canvas::NotifyOnKeyDown(KeyCode);
	//DebugOut(L"Keydown\n");
	auto sceneManager = Game::GetInstance().GetService<SceneManager>();
	Scene* activeScene = sceneManager->GetActiveScene();

	if (activeScene == nullptr) return;

	auto gameObjects = activeScene->GetActiveObjects();

	for (auto go : gameObjects)
	{
		if (go == nullptr) continue;
		if (go->IsDestroyed()) continue;
		if (!go->IsEnabled()) continue;
		
		try
		{
			go->OnKeyDown(KeyCode);
		}
		catch(...) 
		{
			DebugOut(L"[WARNING] Object was destroyed but you try to access\n");
		}
	}
}

void CGlobalKeyEventHandler::OnKeyUp(int KeyCode)
{
	auto sceneManager = Game::GetInstance().GetService<SceneManager>();
	Scene* activeScene = sceneManager->GetActiveScene();
	auto gameObjects = activeScene->GetActiveObjects();

	if (activeScene == nullptr) return;

	for (auto go : gameObjects)
	{
		if (go == nullptr) continue;
		if (go->IsDestroyed()) continue;
		if (!go->IsEnabled()) continue;

		try
		{
			go->OnKeyUp(KeyCode);
		}
		catch (...)
		{
			DebugOut(L"[WARNING] Object was destroyed but you try to access\n");
		}
	}
}
