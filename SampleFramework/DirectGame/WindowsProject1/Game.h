#pragma once
#ifndef GAME_H
#define GAME_H

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <unordered_map>
#include "ServiceLocator.h"
#include "TextureManager.h"
#include "SceneManager.h"
#include "Sprite.h"
#include "InputHandler.h"
#include "Raycast.h"
#include "GlobalConfigs.h"
#include "Font.h"
#include "GameData.h"

using namespace std;

class Game : public ServiceLocator
{
public:
	static void SetTimeScale(float time) { Game::timeScale = time; }
	static float GetTimeScale() { return timeScale * pause; }
	static Game& GetInstance();
	static DWORD DeltaTime();
	static DWORD FixedDeltaTime();

	Raycast* Raycast2D();
	GameObject FindGameObjectWithTag(ObjectTags tag, bool inactiveIncluded = false);

	template<typename T>
	std::vector<T*> FindGameObjectsOfType();

	void SetGlobalConfigs(GlobalConfigs conf);
	GlobalConfigs GetGlobalConfigs();
	
	void InitDirectX(HWND hWnd, int scrWidth, int scrHeight, int fps);
	bool ImportConfig();

	void GameInit(HWND hWnd);
	void GameRun(HWND hWnd);
	void GameEnd();
	void Request();
	void InputProc();
	void Update();
	void Render();
	void Clean();

	void Pause();
	bool IsPaused();

	void SetClearColor(D3DXCOLOR color);

	LPDIRECT3DDEVICE9 GetD3DDevice() { return d3ddev; }

	LPDIRECT3DTEXTURE9 LoadTexture(LPCWSTR texPath);
	void DrawTexture(float x, float y, int xPivot, int yPivot, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, Vector2 scale, float rotation, int alpha = 255);
	void DrawTexture(float x, float y, int xPivot, int yPivot, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);

	std::string GetSourcePathOf(std::string category, std::string id);
	Font* GetGlobalFont();

	GameData* GetData();
	void ModifyData(int world = -1, int score = -1, int life = -1, int coin = -1);

	void GainComboChain(Vector2 position);

	~Game();

private:
	static Game* instance;
	static DWORD deltaTime;
	static float timeScale;
	static int pause;

	D3DXCOLOR clearColor;

	LPDIRECT3D9 d3d = NULL;
	LPDIRECT3DDEVICE9 d3ddev = NULL;

	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;
	
	GlobalConfigs configs;

	SceneManager* sceneManager;
	Scene* activeScene;
	InputHandler* inputHandler;
	Raycast* raycast;
	Font* globalFont;
	GameData* globalData;

	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> sourcePaths;
};
#endif

template<typename T>
inline std::vector<T*> Game::FindGameObjectsOfType()
{
	std::vector<T*> result;

	if (sceneManager == nullptr) sceneManager = GetService<SceneManager>();
	if (sceneManager == nullptr) return result;
	auto scene = sceneManager->GetActiveScene();
	if (scene == nullptr)
	{
		DebugOut(L"SceneNULL"); return result;
	}

	auto searchPool = *scene->GetSceneObjects();
	for (auto o : searchPool)
	{
		T* cast = dynamic_cast<T*>(o);
		if (cast != nullptr && !o->IsDestroyed()) result.push_back(cast);
	}

	return result;
}
