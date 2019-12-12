/*====================================================================

Window�̕\��


�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�쐬�ҁ@���T ����
�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�쐬���@05��08���@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@
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
�萔��`
-------------------------------------------------------------------*/
#define CLASS_NAME   "GameWindow"
#define WINDOW_CAPTION "�t�@�C������"
#define FPS_MEASUREMENT_TIME (1.0)  //���b��FPS���擾���邩

/*------------------------------------------------------------------
�v���g�^�C�v�錾
------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static bool Init(HWND hWnd);
static void Uninit(void); 
static void Update(void);
static void Draw(void);

/*==================================================================
�O���[�o���ϐ�
===================================================================*/
static LPDIRECT3DDEVICE9 g_pDevice = NULL;

static int g_FrameCount = 0;        //�t���[���J�E���^�[
static int g_FPSBaseFrameCount = 0; //FPS�v���p�t���[���J�E���^�[
static double g_FPSBaseTime = 0;    //FPS�v���p����
static float g_FPS = 0.0f;          //FPS

static double g_StaticFrametime = 0.0;

/*----------------------------------------------------------------
�@���C��
----------------------------------------------------------------*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);  //�x�����o���Ȃ��悤�ɂ���
	UNREFERENCED_PARAMETER(lpCmdLine);  //�x�����o���Ȃ��悤�ɂ���

	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = CLASS_NAME;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);

	RegisterClass(&wc);

	int window_style = WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX);

	RECT window_rect{ 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	AdjustWindowRect(&window_rect, window_style, FALSE);  //�E�B���h�E�T�C�Y���ǂꂮ�炢�ɂ����炢���̂����ׂ�

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

	Keyboard_Initialize(hInstance, hWnd);//�L�[�{�[�h���͂����邽�߂̏�����

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
				if (MessageBox(hWnd, "�{���ɏI�����Ă�낵���ł����H", "�m�F", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
					DestroyWindow(hWnd);
				}
				return 0;

		case WM_DESTROY:
					PostQuitMessage(0);
					return 0;
	};

	return DefWindowProc(hWnd, uMsg, wParam, lParam);  //���ʂ̓��������Ă���/�I�������͂ł��Ȃ�
}

bool Init(HWND hWnd)
{
	InitSound(hWnd);

	MyDirect3D_Init(hWnd);
	
	g_pDevice = MyDirect3D_GetDevice();
	g_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);  //�@���͍s��̉e�����󂯂Ȃ�����
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
	//�A�b�v�f�[�g�֐��̂ł��邾���Ō�ɏ���
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
	g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(5,255,255,0), 1.0f, 0);  //��ʂ̃N���A
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