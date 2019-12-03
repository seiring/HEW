/*====================================================================

Windowの表示


　　　　　　　　　　　　　　　　　　　　　　作成者　金乃 寿興
　　　　　　　　　　　　　　　　　　　　　　作成日　05月08日　　　　　　　　　　　　　　　　　　　　　　　　
====================================================================*/
#include<windows.h>
#include<d3d9.h>
#include<d3dx9.h>
#include<math.h>
#include"common.h"
#include"mydirect3d.h"
#include"texture.h"
#include"sprite.h"
#include"debug_font.h"
#include"system_timer.h"
#include"SpriteAnim.h"
#include"cube.h"
#include"grid.h"
#include"light.h"
#include"input.h"
#include"camerar.h"
#include"game.h"
#include"scene.h"
#include"sound.h"

/*-------------------------------------------------------------------
定数定義
-------------------------------------------------------------------*/
#define CLASS_NAME   "GameWindow"
#define WINDOW_CAPTION "ファイル分割"
#define FPS_MEASUREMENT_TIME (1.0)  //何秒でFPSを取得するか

/*------------------------------------------------------------------
プロトタイプ宣言
------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static bool Init(HWND hWnd);
static void Uninit(void); 
static void Update(void);
static void Draw(void);

/*==================================================================
グローバル変数
===================================================================*/
static LPDIRECT3DDEVICE9 g_pDevice = NULL;

static int g_FrameCount = 0;        //フレームカウンター
static int g_FPSBaseFrameCount = 0; //FPS計測用フレームカウンター
static double g_FPSBaseTime = 0;    //FPS計測用時間
static float g_FPS = 0.0f;          //FPS

static double g_StaticFrametime = 0.0;

/*----------------------------------------------------------------
　メイン
----------------------------------------------------------------*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);  //警告を出さないようにする
	UNREFERENCED_PARAMETER(lpCmdLine);  //警告を出さないようにする

	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = CLASS_NAME;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);

	RegisterClass(&wc);

	int window_style = WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX);

	RECT window_rect{ 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	AdjustWindowRect(&window_rect, window_style, FALSE);  //ウィンドウサイズをどれぐらいにしたらいいのか調べる

	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	int window_x = (desktop_width - window_width) / 2;
	int window_y = (desktop_height - window_height) / 2;

	HWND hWnd = CreateWindow(
		CLASS_NAME,
		WINDOW_CAPTION,
		window_style,
		window_x,
		window_y,
		window_width,
		window_height,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	Keyboard_Initialize(hInstance, hWnd);//キーボード入力をするための初期化

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	Init(hWnd);
	
	MSG msg = {};
	while (WM_QUIT != msg.message) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			double time = SystemTimer_GetTime();
			if (time - g_StaticFrametime < 1.0f / 60)
			{
				Sleep(0);
			}
			else
			{
				g_StaticFrametime = time;
				Update();
				Draw();
				if (Scene_Change())
				{
					break;
				}
			}
		}
	}
	Uninit();
	return(int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE) {
				SendMessage(hWnd, WM_CLOSE, 0, 0);
			}
			break;

		case WM_CLOSE:
				if (MessageBox(hWnd, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
					DestroyWindow(hWnd);
				}
				return 0;

		case WM_DESTROY:
					PostQuitMessage(0);
					return 0;
	};

	return DefWindowProc(hWnd, uMsg, wParam, lParam);  //普通の動きをしてくれ/終了処理はできない
}

bool Init(HWND hWnd)
{
	InitSound(hWnd);

	MyDirect3D_Init(hWnd);
	
	g_pDevice = MyDirect3D_GetDevice();
	g_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);  //法線は行列の影響を受けなくする
	DebugFont_Initialize();
	SystemTimer_Initialize();
	SystemTimer_Start();
	g_FrameCount = g_FPSBaseFrameCount = 0;
	g_FPSBaseTime = SystemTimer_GetTime();
	g_FPS = 0.0f;
	Texture_Init();
	Camera_Init();
	Scene_Init();
	Texture_Load();
	Set_Light();
	
	//Grid_Init();

	return true;
}

void Uninit(void)
{
	DebugFont_Finalize();
	MyDirect3D_Uninit();
	Scene_Uninit();
	UninitSound();
}

void Update(void)
{
	Keyboard_Update();
	Scene_Update();
	//アップデート関数のできるだけ最後に書く
	g_FrameCount++;
	double time = SystemTimer_GetTime();
	if (time - g_FPSBaseTime >= FPS_MEASUREMENT_TIME) {
		g_FPS = (float)((g_FrameCount - g_FPSBaseFrameCount) / (time - g_FPSBaseTime));
		g_FPSBaseTime = time;
		g_FPSBaseFrameCount = g_FrameCount;
	}
}

void Draw(void)
{
	g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(255,255,255,0), 1.0f, 0);  //画面のクリア
	g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	g_pDevice->SetSamplerState(0,D3DSAMP_MAXANISOTROPY, 16);
	g_pDevice->BeginScene();
	Scene_Draw();
	//Grid_Draw();
	g_pDevice->EndScene();

	g_pDevice->Present(NULL, NULL, NULL, NULL);
}   