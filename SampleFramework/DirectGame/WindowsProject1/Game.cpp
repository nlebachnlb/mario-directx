#include "Game.h"
#include "Utils.h"
#include "ExampleScene.h"
#include "WorldMapScene.h"
#include "AnimationDatabase.h"
#include "tinyxml.h"
#include "Canvas.h"
#include "MainCanvas.h"
#include "EffectPool.h"
#include "ScoreFX.h"
#include "MainMenuScene.h"
#include "IntroScene.h"
// #include "GameObject.h"


Game* Game::instance = nullptr;
DWORD Game::deltaTime = 0;
int Game::pause = 1;
float Game::timeScale = 1.0f;

Game& Game::GetInstance()
{
	if (instance == nullptr) instance = new Game();
	return *instance;
}

DWORD Game::DeltaTime()
{
	return deltaTime * pause;
}

DWORD Game::FixedDeltaTime()
{
	return 20;
}

Raycast* Game::Raycast2D()
{
	auto scene = sceneManager->GetActiveScene();
	if (scene == nullptr)
	{
		if (raycast != nullptr) delete raycast;
		raycast = nullptr;
	}
	else
	{
		if (raycast == nullptr)
			raycast = new Raycast();
		raycast->SetCastObjects(&scene->GetActiveObjects());
	}

	return raycast;
}

GameObject Game::FindGameObjectWithTag(ObjectTags tag, bool inactiveIncluded)
{
	if (sceneManager == nullptr) sceneManager = GetService<SceneManager>();
	if (sceneManager == nullptr) return nullptr;
	auto scene = sceneManager->GetActiveScene();
	if (scene == nullptr)
	{
		DebugOut(L"SceneNULL"); return nullptr;
	}

	auto searchPool = inactiveIncluded ? *scene->GetSceneObjects() : scene->GetActiveObjects();
	for (auto o : searchPool)
	{
		// DebugOut(L"%d-", o->GetTag());
		if (o->GetTag() == tag) return o;
	}

	// DebugOut(L"PP: %d-", tag);

	return nullptr;
}

void Game::InitDirectX(HWND hWnd, int scrWidth, int scrHeight, int fps)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	configs.fps = fps;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = configs.screenWidth = scrWidth;
	d3dpp.BackBufferHeight = configs.screenHeight = scrHeight;
	d3dpp.Flags = 0;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = NULL;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;
	d3dpp.hDeviceWindow = hWnd;

	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &d3ddev);

	if (!d3ddev)
	{
		MessageBox(hWnd, L"Creating device failed!", L"Error", MB_OK | MB_ICONERROR);
		return;
	}

	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	D3DXCreateSprite(d3ddev, &spriteHandler);

	if (!spriteHandler)
	{
		MessageBox(hWnd, L"Creating sprite handler failed!", L"Error", MB_OK | MB_ICONERROR);
		return;
	}
}

bool Game::ImportConfig()
{
	auto path = "Assets/root.xml";

	TiXmlDocument document(path);
	if (document.LoadFile() == false)
	{
		OutputDebugStringW(ToLPCWSTR(document.ErrorDesc()));
		return false;
	}

	TiXmlElement* root = document.RootElement();
	for (TiXmlElement* element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
	{
		std::string category = element->Attribute("name");
		std::unordered_map<std::string, std::string> bucket;

		// OutputDebugStringW(ToLPCWSTR(category + '\n'));
		for (auto item = element->FirstChildElement(); item != nullptr; item = item->NextSiblingElement())
		{
			std::string id = item->Attribute("id");
			std::string source = item->Attribute("source");
			bucket.insert(make_pair(id, source));
			// OutputDebugStringW(ToLPCWSTR("|--" + id + ":" + source + '\n'));
		}

		sourcePaths.insert(make_pair(category, bucket));
	}

	// OutputDebugStringW(ToLPCWSTR("Test:" + GetSourcePathOf("Textures", "tex-mario") + '\n'));
	return true;
}

void Game::SetGlobalConfigs(GlobalConfigs conf)
{
	configs = conf;
}

GlobalConfigs Game::GetGlobalConfigs()
{
	return configs;
}

void Game::GameInit(HWND hWnd)
{
	raycast = nullptr;
#pragma region Init Services
	DebugOut(L"Init Service\n");
	// Texture Manager
	TextureManager* textureManager = new TextureManager();
	textureManager->Initialization();
	AddService(textureManager);

	// Sprite Manager
	SpriteManager* spriteManager = new SpriteManager();
	spriteManager->Initialization();
	AddService(spriteManager);

	// Scene Manager
	SceneManager* sceneManager = new SceneManager();
	sceneManager->Initialization();
	AddService(sceneManager);
	this->sceneManager = sceneManager;

	// Animation Database
	AnimationDatabase* animationDb = new AnimationDatabase();
	animationDb->Initialization();
	AddService(animationDb);

	// Input Handler
	InputHandler* inputHandler = new InputHandler();
	inputHandler->SetHandleWindow(hWnd);
	CGlobalKeyEventHandler* keyboardHandler = new CGlobalKeyEventHandler();
	inputHandler->SetKeyEventHandler(keyboardHandler);
	inputHandler->Initialization();
	AddService(inputHandler);
	this->inputHandler = inputHandler;

	// Game map
	GameMap* gameMap = new GameMap();
	gameMap->Initialization();
	AddService(gameMap);

	// Test Get Service
	DebugOut(L"TextureManager service: %d\n", GetService<TextureManager>() != nullptr ? 1 : 0);
	DebugOut(L"SpriteManager service: %d\n", GetService<SpriteManager>() != nullptr ? 1 : 0);
	DebugOut(L"SceneManager service: %d\n", GetService<SceneManager>() != nullptr ? 1 : 0);
#pragma endregion

#pragma region Init Canvas Layout
	// Init game font
	globalFont = new Font();
	vector<FontSprite> fontSet;
	std::string prefix = "spr-font-";
	for (char c = '0'; c <= '9'; ++c)
		fontSet.push_back(FontSprite{ c, prefix + c });
	for (char c = 'A'; c <= 'Z'; ++c)
		fontSet.push_back(FontSprite{ c, prefix + c });
	fontSet.push_back(FontSprite{ '!', "spr-font-exclamation-point" });
	globalFont->Import(fontSet);

	MainCanvas* mainCanvas = new MainCanvas();
	mainCanvas->Initialize();
	GameData* dataNode = new GameData();
	globalData = dataNode;
	mainCanvas->SetGameData(dataNode);
	Canvas::AddCanvas("main", mainCanvas);
#pragma endregion

#pragma region Init Render Options
	// Disable ZBuffer, our 2D game already has the renderOrder itself
	d3ddev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
	d3ddev->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
#pragma endregion

	sceneManager->LoadScene(new IntroScene());

	SetTimeScale(1.0f);
}

void Game::GameRun(HWND hWnd)
{
	MSG msg;
	bool done = false;

	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / configs.fps;
	deltaTime = 0;

	// Game Loop
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				done = true;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			auto now = GetTickCount();
			deltaTime = (now - frameStart);

			// Update & Render in limited fps
			if (deltaTime >= tickPerFrame)
			{
				frameStart = now;

				if (sceneManager == nullptr) sceneManager = GetService<SceneManager>();

				// Process load scene requests first
				sceneManager->ProcessLoadRequests();

				// Process instantiate requests at last frame
				Request();
				// Process input
				InputProc();
				// Start updating new frame
				Update();
				// Render new frame
				Render();
				// Clean destroyed objects
				Clean();

				// Process unload scene requests first
				sceneManager->ProcessUnloadRequests();

				Sleep(0);
			}
			else
				Sleep(tickPerFrame - deltaTime);
		}
	}
}

void Game::GameEnd()
{
	if (!d3d) d3d->Release();
	if (!d3ddev) d3ddev->Release();
	if (!backBuffer) backBuffer->Release();
	if (!spriteHandler) spriteHandler->Release();
	Canvas::Clean();
}

void Game::Request()
{
	auto activeScene = sceneManager->GetActiveScene();

	if (activeScene != nullptr)
		activeScene->ProcessInstantiateRequests(),
		activeScene->UpdateActiveObjects();

	Canvas::ProcessAddRequests();
}

void Game::InputProc()
{
	if (sceneManager->GetActiveScene() == nullptr) return;
	if (inputHandler == nullptr) inputHandler = GetService<InputHandler>();
	inputHandler->ProcessKeyboard();
}

void Game::Update()
{
	auto activeScene = sceneManager->GetActiveScene();

	if (activeScene != nullptr)
		activeScene->Update();

	Canvas::UpdateAllCanvases();
}

void Game::Render()
{
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, clearColor, 1.0f, 0);

	// if (SUCCEEDED(d3ddev->BeginScene()))
	d3ddev->BeginScene();
	{
		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
		{
			auto activeScene = sceneManager->GetActiveScene();
			if (activeScene != nullptr)
				activeScene->Render();

			Canvas::RenderAllCanvases();
		}
		spriteHandler->End();
	}
	d3ddev->EndScene();

	d3ddev->Present(NULL, NULL, NULL, NULL);
}

void Game::Clean()
{
	auto activeScene = sceneManager->GetActiveScene();

	if (activeScene != nullptr)
		activeScene->CleanDestroyedObjects();
}

void Game::Pause()
{
	pause = 1 - pause;
}

bool Game::IsPaused()
{
	return pause == 0;
}

void Game::SetClearColor(D3DXCOLOR color)
{
	this->clearColor = color;
}

LPDIRECT3DTEXTURE9 Game::LoadTexture(LPCWSTR texPath)
{
	D3DXIMAGE_INFO info;
	LPDIRECT3DTEXTURE9 texture;

	auto result = D3DXGetImageInfoFromFile(texPath, &info);

	if (result != D3D_OK) return NULL;

	result = D3DXCreateTextureFromFileEx(
		d3ddev, texPath, info.Width, info.Height,
		1, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT,
		D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
		&info, NULL, &texture
	);

	if (result != D3D_OK) return NULL;
	return texture;
}

void Game::DrawTexture(float x, float y, int xPivot, int yPivot, LPDIRECT3DTEXTURE9 texture, 
	int left, int top, int right, int bottom, 
	Vector2 scale, float rotation, int alpha)
{
	if (scale == Vector2(1, 1) && rotation < 0.001f)
	{
		DrawTexture(x, y, xPivot, yPivot, texture, left, top, right, bottom, alpha);
		return;
	}

	RECT rect;
	rect.left = left;
	rect.top = top;
	rect.right = right;
	rect.bottom = bottom;

	D3DXMATRIX oldMatrix, newMatrix;
	spriteHandler->GetTransform(&oldMatrix);

	D3DXMatrixTransformation2D(&newMatrix, &Vector2((int)x, (int)y), 0, &scale, &Vector2((int)x, (int)y), rotation, &VectorZero());

	spriteHandler->SetTransform(&newMatrix);
	spriteHandler->Draw(texture, &rect, &Vector3(xPivot, yPivot, 0), &Vector3((int)x, (int)y, 0), D3DCOLOR_ARGB(alpha, 255, 255, 255));
	spriteHandler->SetTransform(&oldMatrix);
}

void Game::DrawTexture(float x, float y, int xPivot, int yPivot, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha)
{
	RECT rect;
	rect.left	= left;
	rect.top	= top;
	rect.right	= right;
	rect.bottom = bottom;

	spriteHandler->Draw(texture, &rect, &Vector3(xPivot, yPivot, 0), &Vector3((int)x, (int)y, 0), D3DCOLOR_ARGB(alpha, 255, 255, 255));
}

std::string Game::GetSourcePathOf(std::string category, std::string id)
{
	if (sourcePaths.find(category) != sourcePaths.end())
	{
		auto bucket = sourcePaths.at(category);
		if (bucket.find(id) != bucket.end())
			return bucket.at(id);
		else
			return "";
	}

	return "";
}

Font* Game::GetGlobalFont()
{
	return this->globalFont;
}

GameData* Game::GetData()
{
	return globalData;
}

void Game::ModifyData(int world, int score, int life, int coin)
{
	if (world != -1) globalData->world = world;
	if (score != -1) globalData->score = score;
	if (life != -1) globalData->life = life;
	if (coin != -1) globalData->coin = coin;
}

void Game::GainComboChain(Vector2 position)
{
	auto gmap = GetService<GameMap>();
	auto spawner = gmap->GetSpawnerManager();
	auto fxPool = spawner != nullptr ? spawner->GetService<EffectPool>() : nullptr;
	
	if (fxPool != nullptr)
	{
		ScoreFX* fx = static_cast<ScoreFX*>(fxPool->CreateFX("fx-score", position));
		globalData->GainCombo();
		fx->SetLevel((Score)globalData->GetCombo());
	}
}

Game::~Game()
{
	
}
