#include<d3dx9.h>
#include"mydirect3d.h"
#include"common.h"
#include"texture.h"
#include"sprite.h"
#include "uzu.h"
#include "system_timer.h"
#include"input.h"
#include"collision.h"
#include"debug_font.h"

#define UZU_HEIGHT 491
#define UZU_WIDTH 348

typedef struct UzuVertex_tag
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;  //法線
	D3DCOLOR color;
	D3DXVECTOR2 uv;
	Circle collision;
	bool used;
}UzuVertex;

#define FVF_UZU (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

static UzuVertex g_Uzus[UZU_MAX];

static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;

static float g_angle_x = 0.0f;
static float g_angle_y = 0.0f;

static int g_texid = -1;
static int g_texid2 = -1;

static int framecount = 0;

static UzuVertex g_cube_vertex[] = {
	//1個目(前)
	{D3DXVECTOR3(-0.8f, 1.0f,1.6f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.38f,0.2f)},
	{D3DXVECTOR3( 0.8f, 1.0f,1.6f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.55f,0.2f)},
	{D3DXVECTOR3(-0.8f, 1.0f,0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.38f,0.35f)},
	{D3DXVECTOR3( 0.8f, 1.0f,0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.55f,0.35f)},
};


void UzuInit()
{
	g_texid = Texture_SetLoadFile("Asset/Texture/uzu.png", UZU_WIDTH, UZU_HEIGHT);
	//g_texid2 = Texture_SetLoadFile("Asset/Texture/uzu反転.png", UZU_WIDTH, UZU_HEIGHT);
	Texture_Load();

	framecount = 0;

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	pDevice->CreateVertexBuffer(sizeof(UzuVertex) * 4, D3DUSAGE_WRITEONLY, FVF_UZU, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);
	pDevice->CreateIndexBuffer(sizeof(WORD) * 6, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIndexBuffer, NULL);


	UzuVertex *pv;

	g_pVertexBuffer->Lock(0, 0, (void**)&pv, D3DLOCK_DISCARD);
	//                      ↑全部       ↑仮想アドレス
	memcpy(pv, g_cube_vertex, sizeof(g_cube_vertex));

	g_pVertexBuffer->Unlock(); //Unlockをしたら仮想アドレスは使えない



	WORD *pIndex;
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD);

	pIndex[0] = 0, pIndex[1] = 1, pIndex[2] = 2;
	pIndex[3] = 1, pIndex[4] = 3, pIndex[5] = 2;

	g_pIndexBuffer->Unlock();

	

	for (int i = 0; i <= UZU_MAX; i++)
	{
		g_Uzus[i].used = false;
	}
}

void UzuUninit()
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

void UzuUpdate()
{
	for (int i = 0; i <= UZU_MAX; i++)
	{
		if (g_Uzus[i].used)
		{
			g_Uzus[i].collision.position.x = g_Uzus[i].position.x;
			g_Uzus[i].collision.position.y = g_Uzus[i].position.z + 1.0f;
			g_Uzus[i].position.z -= 0.01f;
		}
	}

	for (int i = 0; i <= UZU_MAX; i++)
	{
		if (g_Uzus[i].position.z < -8.0f)
		{
			g_Uzus[i].used = false;
		}
	}
	framecount++;
}



void UzuDraw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();  //Deviceの取得
	//ワールド変換行列
	D3DXMATRIX mtxWorld,mtxRotation, mtxRotationY, mtxRotationX, mtxTranslation, mtxScaling, mtxTranslation2;
	D3DXMatrixIdentity(&mtxWorld);  //単位行列にする

	pDevice->SetFVF(FVF_UZU);

	

	mtxWorld =  mtxTranslation;

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld); //デバイスにワールド変換行列を設定
	pDevice->SetTexture(0, Texture_GetTexture(g_texid));

	for (int i = 0; i <= UZU_MAX; i++)
	{
		if (g_Uzus[i].used)
		{
			D3DXMatrixTranslation(&mtxTranslation, g_Uzus[i].position.x, g_Uzus[i].position.y, g_Uzus[i].position.z);
			mtxWorld = mtxTranslation;
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld); //デバイスにワールド変換行列を設定
			pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(UzuVertex));
			pDevice->SetIndices(g_pIndexBuffer);
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
		}
	}
	/*DebugFont_Draw(0, 0,  "%f  %f  %f", g_Uzus[0].position.x, g_Uzus[0].position.y, g_Uzus[0].position.z);
	DebugFont_Draw(0, 30, "%f  %f"    , g_Uzus[0].collision.position.x, g_Uzus[0].collision.position.y);
	DebugFont_Draw(0, 60, "%f  %f  %f", g_Uzus[1].position.x, g_Uzus[1].position.y, g_Uzus[1].position.z);
	DebugFont_Draw(0, 90, "%f  %f"    , g_Uzus[1].collision.position.x, g_Uzus[1].collision.position.y);*/
}

void Uzu_Create(float x, float z)
{
	for (int i = 0; i <= UZU_MAX; i++)
	{
		g_Uzus[i].collision.radius = 0.5f;
	}

	for (int i = 0; i <= UZU_MAX; i++)
	{
		g_Uzus[i].collision.position.x = g_Uzus[i].position.x;
		g_Uzus[i].collision.position.y = g_Uzus[i].position.z;
	}

	for (int i = 0; i <= UZU_MAX; i++)
	{
		if (g_Uzus[i].used == true)
		{
			continue;
		}
		g_Uzus[i].position.x = x;
		g_Uzus[i].position.y = 0.0f;
		g_Uzus[i].position.z = z;
		g_Uzus[i].used = true;
		break;
	}
}

const Circle* Uzu_GetCircleCollision(int i)
{
	return &g_Uzus[i].collision;
}

bool Uzu_IsUsed(int i)
{
	return g_Uzus[i].used;
}


void Uzu_Destroy(int i)
{
	g_Uzus[i].used = false;
}

float GET_UzuX(int i)
{
	return g_Uzus[i].position.x;
}
float GET_UzuZ(int i)
{
	return g_Uzus[i].position.z;
}
float Get_count()
{
	return framecount;
}