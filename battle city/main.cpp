#include <windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <vector>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"

#define WINDOW_CLASS_NAME L"Game Window"
#define MAIN_WINDOW_TITLE L"Battle City"
#define WINDOW_ICON_PATH L"img/logo.ico"

#define TEXTURE_PATH_WHITE_LEFT L"img/white_left.png"
#define TEXTURE_PATH_WHITE_RIGHT L"img/white_right.png"
#define TEXTURE_PATH_WHITE_UP L"img/white_up.png"
#define TEXTURE_PATH_WHITE_DOWN L"img/white_down.png"

#define TEXTURE_PATH_GREEN_LEFT L"img/green_left.png"
#define TEXTURE_PATH_GREEN_RIGHT L"img/green_right.png"
#define TEXTURE_PATH_GREEN_UP L"img/green_up.png"
#define TEXTURE_PATH_GREEN_DOWN L"img/green_down.png"

#define TEXTURE_PATH_RED_LEFT L"img/red_left.png"
#define TEXTURE_PATH_RED_RIGHT L"img/red_right.png"
#define TEXTURE_PATH_RED_UP L"img/red_up.png"
#define TEXTURE_PATH_RED_DOWN L"img/red_down.png"

#define TEXTURE_PATH_YELLOW_LEFT L"img/yellow_left.png"
#define TEXTURE_PATH_YELLOW_RIGHT L"img/yellow_right.png"
#define TEXTURE_PATH_YELLOW_UP L"img/yellow_up.png"
#define TEXTURE_PATH_YELLOW_DOWN L"img/yellow_down.png"

#define TEXTURE_PATH_BULLET L"img/bullet.png"

#define BACKGROUND_COLOR D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240


using namespace std;

Player* player;
#define PLAYER_START_X 156.0f
#define PLAYER_START_Y 192.0f
#define PLAYER_START_VX 0.1f
#define PLAYER_START_VY 0.1f

vector<Enemy*> enemies;
vector<LPGAMEOBJECT> objects;

LPTEXTURE texLeftPlayer = NULL;
LPTEXTURE texRightPlayer = NULL;
LPTEXTURE texUpPlayer = NULL;
LPTEXTURE texDownPlayer = NULL;

LPTEXTURE texBullet = NULL;
LPTEXTURE texEnemy = NULL;

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources. In this example, create a brick object and mario object
*/
void LoadResources()
{
	CGame* game = CGame::GetInstance();
	texBullet = game->LoadTexture(TEXTURE_PATH_BULLET);
	texLeftPlayer = game->LoadTexture(TEXTURE_PATH_YELLOW_LEFT);
	texRightPlayer = game->LoadTexture(TEXTURE_PATH_YELLOW_RIGHT);
	texUpPlayer = game->LoadTexture(TEXTURE_PATH_YELLOW_UP);
	texDownPlayer = game->LoadTexture(TEXTURE_PATH_YELLOW_DOWN);

	player = new Player(PLAYER_START_X, PLAYER_START_Y, PLAYER_START_VX, PLAYER_START_VY,
		texLeftPlayer, texRightPlayer, texUpPlayer, texDownPlayer,
		0, 0, 0, 0, texBullet, NULL, NULL, NULL);
	/*bullet = new CBullet(BULLET_X, BULLET_Y, BULLET_VX, BULLET_VY, texBullet);
	monster = new CMonster(MONSTER_X, MONSTER_Y, MONSTER_VX, MONSTER_VY, texMonster);*/
	game->AddPlayer(player);
	objects.push_back(player);

	/*objects.push_back(brick);
	for (int i = 0; i < 20; i++) {
		objects.push_back(new CBrick(BRICK_X + i * 16, BRICK_Y, texBrick));
	}*/
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	for (LPGAMEOBJECT obj : objects)
		obj->Update(dt);

	DebugOutTitle(L"01 - Skeleton %0.1f, %0.1f", player->GetX(), player->GetY());
}

/*
	Render a frame
*/
void Render()
{
	CGame* g = CGame::GetInstance();

	ID3D10Device* pD3DDevice = g->GetDirect3DDevice();
	IDXGISwapChain* pSwapChain = g->GetSwapChain();
	ID3D10RenderTargetView* pRenderTargetView = g->GetRenderTargetView();
	ID3DX10Sprite* spriteHandler = g->GetSpriteHandler();

	if (pD3DDevice != NULL)
	{
		// clear the background 
		pD3DDevice->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DX10_SPRITE_SORT_TEXTURE);

		// Use Alpha blending for transparent sprites
		FLOAT NewBlendFactor[4] = { 0,0,0,0 };
		pD3DDevice->OMSetBlendState(g->GetAlphaBlending(), NewBlendFactor, 0xffffffff);

		for (LPGAMEOBJECT obj : objects)
			obj->Render();

		// Uncomment this line to see how to draw a porttion of a texture  
		//g->Draw(10, 10, texMisc, 300, 117, 317, 134);
		//g->Draw(10, 10, texMario, 215, 120, 234, 137);

		spriteHandler->End();
		pSwapChain->Present(0, 0);
	}
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		DWORD ErrCode = GetLastError();
		DebugOut(L"[ERROR] CreateWindow failed! ErrCode: %d\nAt: %s %d \n", ErrCode, _W(__FILE__), __LINE__);
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	SetDebugWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	ULONGLONG frameStart = GetTickCount64();
	ULONGLONG tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ULONGLONG now = GetTickCount64();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		ULONGLONG dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;
			Update((DWORD)dt);
			Render();
		}
		else
			Sleep((DWORD)(tickPerFrame - dt));
	}

	return 1;
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	CGame* game = CGame::GetInstance();
	game->Init(hWnd);

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);


	LoadResources();

	Run();

	return 0;
}