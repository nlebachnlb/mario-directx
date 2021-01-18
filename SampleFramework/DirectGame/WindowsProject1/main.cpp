// WindowsProject1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Consts.h"
#include "tinyxml.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
HWND                InitInstance(HINSTANCE, int, int, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    Game::GetInstance().ImportConfig();
    auto path = Game::GetInstance().GetSourcePathOf(CATEGORY_CONFIG, CG_GLOBAL_CONFIG);
    int fps;
    int resWidth, resHeight, hudOffset;

    TiXmlDocument document(path.c_str());
    if (document.LoadFile() == false)
    {
        OutputDebugStringW(ToLPCWSTR(document.ErrorDesc()));
        return FALSE;
    }

    TiXmlElement* root = document.RootElement();
    for (TiXmlElement* element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
    {
        std::string name = element->Attribute("name");
        if (name.compare("frame-rate") == 0)
            element->QueryIntAttribute("value", &fps);
        else if (name.compare("resolution") == 0)
        {
            element->QueryIntAttribute("width", &resWidth);
            element->QueryIntAttribute("height", &resHeight);
        }
        else if (name.compare("hud-offset") == 0)
            element->QueryIntAttribute("value", &hudOffset);
    }

    DebugOut(L"conf: %d, %d, %d\n", fps, resWidth, resHeight);

    MyRegisterClass(hInstance);

    // Perform application initialization:
    HWND hWnd = InitInstance(hInstance, nCmdShow, resWidth, resHeight);
    if (!hWnd)
    {
        return FALSE;
    }

    auto c = Game::GetInstance().GetGlobalConfigs(); c.hudOffset = hudOffset;
    Game::GetInstance().SetGlobalConfigs(c);
    Game::GetInstance().InitDirectX(hWnd, resWidth, resHeight, fps);
    // SetWindowPos(hWnd, 0, 0, 0, resWidth * 0.8f, resHeight * 0.8f, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

    Game::GetInstance().GameInit(hWnd);
    Game::GetInstance().GameRun(hWnd);
    Game::GetInstance().GameEnd(); 

    return 0;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = TITLE;
    wcex.hIconSm = NULL;

    return RegisterClassExW(&wcex);
}

HWND InitInstance(HINSTANCE hInstance, int nCmdShow, int scrWidth, int scrHeight)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(TITLE, TITLE, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, scrWidth, scrHeight, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return NULL;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return hWnd;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}