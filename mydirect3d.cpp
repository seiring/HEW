#include<windows.h>
#include<d3d9.h>
#include<d3dx9.h>
#include "common.h"

static LPDIRECT3D9 g_pD3D = NULL;
static LPDIRECT3DDEVICE9 g_pDevice = NULL;

bool MyDirect3D_Init(HWND hWnd)
{
	LPDIRECT3D9 g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		if (MessageBox(hWnd, "g_pD3DがNULLです", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
		{
			DestroyWindow(hWnd);
		}
	}
	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.BackBufferWidth = SCREEN_WIDTH;  //バックバッファー（裏画面）の幅
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;  //バックバッファーの高さ
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;  //バックバッファの種類
	d3dpp.BackBufferCount = 1;  //バックバッファーの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;  //スワップの方法
	d3dpp.Windowed = TRUE;  //ウィンドウモードにするかフルスクリーンにするか(ウィンドウモードの場合はTRUE)
	d3dpp.EnableAutoDepthStencil = TRUE;  //デプスバッファ（深度情報）・ステンシルバッファを有効にするかどうか　必ずTRUE
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;  //リフレッシュレートの指定（フルスクリーンモード時のみ）
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	HRESULT hr = g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &g_pDevice);

	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	return true;
}

void MyDirect3D_Uninit(void)
{
	if (g_pDevice != NULL)
	{
		g_pDevice->Release();
		g_pDevice = NULL;
	}

	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}
LPDIRECT3DDEVICE9 MyDirect3D_GetDevice(void)
{
	return g_pDevice;
}