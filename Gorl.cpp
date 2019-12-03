#include<d3dx9.h>
#include <stdio.h>
#include"mydirect3d.h"
#include"common.h"
#include"texture.h"
#include"sprite.h"
#include "Gorl.h"
#include "uzu.h"
#include "sound.h"
#include"scene.h"

#define GORL_SIZE	1000
static int time_first = 80;
static int time_end = 85;
static bool g_end = false;
static float con = 0;

typedef struct GorlVertex_tag
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;  //法線
	D3DCOLOR color;
	D3DXVECTOR2 uv;
}GorlVertex;

#define FVF_PLAYER (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;

static bool push = false;

static float g_Angle = 1.0f;

static float g_angle_x = 0.0f;
static float g_angle_y = 0.0f;

static D3DXVECTOR3 g_player;

static int g_texid = -1;

static GorlVertex g_start_vertex[] = {

	//1個目(前)
	{D3DXVECTOR3(-2.0,2.0 ,-2.0),D3DXVECTOR3(0.0f,0.0f,-1.0f) ,D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.5f)},
	{D3DXVECTOR3(2.0, 2.0,-2.0),D3DXVECTOR3(0.0f,0.0f,-1.0f)  ,D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,0.5f)},
	{D3DXVECTOR3(-2.0,-1.0,-2.0),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f)},
	{D3DXVECTOR3(2.0,-1.0,-2.0),D3DXVECTOR3(0.0f,0.0f,-1.0f) ,D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,1.0f)},
};


void InitGorl()
{
	g_end = false;
	g_texid = Texture_SetLoadFile("Asset/Texture/moji.png", GORL_SIZE, GORL_SIZE);
	Texture_Load();
}

void UninitGorl()
{
	if (g_pVertexBuffer)
	{
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}
	if (g_pIndexBuffer)
	{
		g_pIndexBuffer->Release();
		g_pVertexBuffer = NULL;
	}
}

void UpdateGorl()
{
}

void DrawGorl()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();  //Deviceの取得
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE); //ライト未使用
	float counter = Get_count();
	
	if (counter >= 60 * time_first && counter <= 60 * time_end)
	{
		Sprinte_Draw(g_texid, (SCREEN_WIDTH - 1000) / 2, (SCREEN_HEIGHT - 600) / 2, 1000, 500, 1000, 500, 0, 1.0, 1.0, 1.0);
		g_end = true;
		if (con == 0)
		{
			PlaySound(SOUND_LABEL_BGM004);
			con++;
		}
	}
}

bool End()
{
	return g_end;
}