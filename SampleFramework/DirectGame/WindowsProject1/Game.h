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

using namespace std;

struct GlobalConfigs
{
	int fps;
	int screenWidth, screenHeight;
};

class Game : public ServiceLocator
{
public:
	static void SetTimeScale(float time) { Game::timeScale = time; }
	static float GetTimeScale() { return timeScale; }
	static Game& GetInstance();
	static float DeltaTime();
	static float FixedDeltaTime();

	void SetGlobalConfigs(GlobalConfigs conf);
	GlobalConfigs GetGlobalConfigs();
	
	void InitDirectX(HWND hWnd, int scrWidth, int scrHeight, int fps);
	bool ImportConfig();

	void GameInit(HWND hWnd);
	void GameRun(HWND hWnd);
	void GameEnd();
	void Update();
	void Render();

	void SetClearColor(D3DXCOLOR color);

	LPDIRECT3DDEVICE9 GetD3DDevice() { return d3ddev; }

	LPDIRECT3DTEXTURE9 LoadTexture(LPCWSTR texPath);
	void DrawTexture(float x, float y, int xPivot, int yPivot, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, Vector2 scale, float rotation);
	void DrawTexture(float x, float y, int xPivot, int yPivot, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom);

	std::string GetSourcePathOf(std::string category, std::string id);

	~Game();

private:
	static Game* instance;
	static float deltaTime;
	static float timeScale;

	D3DXCOLOR clearColor;

	LPDIRECT3D9 d3d = NULL;
	LPDIRECT3DDEVICE9 d3ddev = NULL;

	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;
	
	GlobalConfigs configs;

	SceneManager* sceneManager;
	InputHandler* inputHandler;

	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> sourcePaths;
};
#endif