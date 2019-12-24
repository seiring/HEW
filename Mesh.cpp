#include"common.h"
#include<d3d9.h>
#include"texture.h"
#include<d3dx9.h>
#include"mydirect3d.h"
#include "camerar.h"

static float g_angle = 0.05f;
static float g_rad = 0.0f;
static 	float g_max = 5.0f;
static int kannri = -1;
static int g_vertex_counter = 0;
static int g_primitive_counter = 0;
static int m = 0;
static float scrool = 0.0f;
int distance[1700] = {};
static int g_kazu_x = 0;
static int g_kazu_y = 0;
#define FVF_MESH (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1) //３DになってHWはいらなくなった 構造体の下に書く

static LPDIRECT3DINDEXBUFFER9 g_index;
static LPDIRECT3DVERTEXBUFFER9 g_Vertex;
static D3DXVECTOR3 source(0, 0, 20);

typedef struct MeshVertex_tag
{
	D3DXVECTOR3 position;
	D3DCOLOR	color;
	D3DXVECTOR2 uv;
}MeshVertex;

MeshVertex *pv;

#define FVF_GRID (D3DFVF_XYZ|D3DFVF_DIFFUSE) //３DになってHWはいらなくなった 構造体の下に書く

void MeshInit(int x, int y)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = MyDirect3D_GetDevice();

	g_kazu_x = x;
	g_kazu_y = y;
	int t = 0;
	//頂点の数カウント
	g_vertex_counter = (x + 1)*(y + 1);

	//使用ポリゴンの数をカウント
	g_primitive_counter = ((x * y) * 2) + (4 * (y - 1));//縮退なし=X*y*2

	MeshVertex* g_mesh_Vertex;//ポインタ型は[]いらない

		//ダブルラップ(UVのラップ)ロックの外
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);//アドレスのU
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);//アドレスのV

	//これがないとg_vertexがNUllになる
	pDevice->CreateVertexBuffer(sizeof(MeshVertex) * g_vertex_counter, D3DUSAGE_WRITEONLY, FVF_MESH, D3DPOOL_MANAGED, &g_Vertex, NULL);//頂点
	//Lock = ほかのファイルがいじらないようにする

	//ロックするg_Vertex
	g_Vertex->Lock(0, 0, (void**)&pv, D3DLOCK_DISCARD);
	// g_mesh_Vertex = new MeshVertex[(x+1)*(y+1)];//頂点数分配列を作る

	float yoko = 0;
	float tate = 0;
	float sea = 0.1;
	yoko = 1 / (float)x; // 一つの大きさｘ
	tate = 1 / (float)y;//一つの大きさｙ
	int  takasa = 0;

	int m = 0;
	//頂点インデックスを作りたい
	for (int i = 0; i < y + 1; i++)
	{
		for (int t = 0; t < x + 1; t++)
		{

			//distance = D3DXVec3LengthSq(&(source - pv[m].position));//二点間の距離を取る
			//ｙ座標　＝　最大の高さ＊　距離＊ 振幅 ＊時間（角度）

			pv[m].position = D3DXVECTOR3(-x / 2 + t, 0.0, -y / 2 + i); //左下　index0　　
			pv[m].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pv[m].uv = D3DXVECTOR2(t * yoko, (y - i) * tate);//ダブルラップ
			distance[m] = D3DXVec3LengthSq(&(source - pv[m].position));//二点間の距離を取る

			m++;
		}
	}

	//アンロックしてる
	g_Vertex->Unlock();//バッファ完成 GPUにあるからいらないと返す

	//インデックスバッファ
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((((x + 1) * 2)*y) + (y - 1) * 2), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_index, NULL);
	WORD *pindex;
	g_index->Lock(0, 0, (void**)&pindex, D3DLOCK_DISCARD);
	t = 0;
	//インデックスを作る(縮退含め)
	int index = 0;
	index = (((x + 1) * 2)*y) + (y - 1) * 2;

	//pindexの中身を見るためのaを同じ数宣言
	int a[(((40 + 1) * 2)*40) + ( 40- 1) * 2];

	for (int i = 0; i < index; i += 2)
	{
		pindex[i] = t;//一列目
		a[i] = t; // pindexの中身を見るためaに入れてる
		pindex[i + 1] = x + 1 + t;//二列目

		a[i + 1] = x + 1 + t;//二列目
		//縮退ポリゴン
		if (t % (x + 1) == x)
		{
			pindex[i + 2] = x + 1 + t;//今の終わり
			a[i + 3] = x + 1 + t;//今の終わり

			pindex[i + 3] = t + 1;//次の行の最初
			a[i + 4] = t + 1;//次の行の最初
			i += 2;

		}
		//t
		t++;
	}

	//pvはポインタだから構造体の宣言はいらない
	//positionの入力
	//pindex[indexの数]　=　「頂点の数」
	//memcpy(pv, g_mesh_Vertex, sizeof(g_mesh_Vertex));
	g_index->Unlock();

	kannri = Texture_SetLoadFile("Asset/Texture/background.png", 842, 545);
	//kannri = Texture_SetLoadFile("", 842, 545);
	Texture_Load();
}

void MeshUninit()
{

}

void MeshUpdate()
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = MyDirect3D_GetDevice();

	g_rad += 0.1f;
	g_angle = sin(g_rad);

	int m = 0;
	float yoko = 0;
	float tate = 0;
	yoko = 1 / (float)g_kazu_x;
	tate = 1 / (float)g_kazu_y;
	float takasa = 0;

	scrool = g_rad * 0.03;

	//これがないとg_vertexがNUllになる
	pDevice->CreateVertexBuffer(sizeof(MeshVertex) * g_vertex_counter, D3DUSAGE_WRITEONLY, FVF_MESH, D3DPOOL_MANAGED, &g_Vertex, NULL);//頂点
	//Lock = ほかのファイルがいじらないようにする

	//ロックするg_Vertex
	g_Vertex->Lock(0, 0, (void**)&pv, D3DLOCK_DISCARD);

	for (int i = 0; i < g_kazu_y + 1; i++)
	{
		for (int t = 0; t < g_kazu_x + 1; t++)
		{
			//頂点を作っている

			//ｙ座標　＝　最大の高さ＊　距離＊ 振幅 ＊時間（角度）
			//takasa = 0.01 * distance[m] * 3 * g_angle;
			takasa = /*(0.01 * distance[m] * 3 * g_angle)*/ +sinf((distance[m] * 0.1) - g_rad)*0.1;//波の増減＊大きさ

			pv[m].position = D3DXVECTOR3(-g_kazu_x / 2 + t, 0.0 + takasa, -g_kazu_y / 2 + i); //左下　index0　　
			pv[m].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pv[m].uv = D3DXVECTOR2(t * yoko, (g_kazu_y - i) * tate - scrool);//ダブルラップ

			m++;
		}
	}
	g_Vertex->Unlock();

}

void MeshDraw()
{

	LPDIRECT3DDEVICE9 pDevice;
	pDevice = MyDirect3D_GetDevice();
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//----------------------------------------
	//ワールド変換
	//----------------------------------------
	D3DXMATRIX mtxWorld, mtxRotation, mtxTranslation;

	//何もしない行列
	D3DXMatrixIdentity(&mtxWorld);
	D3DXMatrixScaling(&mtxWorld, 0.5f, 1.0f, 0.5f);

	Camera_Update();

	pDevice->SetFVF(FVF_MESH);
	pDevice->SetTexture(0, Texture_GetTexture(kannri));

	D3DXMatrixTranslation(&mtxTranslation, 0.0f, 0.5f, 0.0f);
	//mtxWorld = mtxTranslation;


	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);// デバイスにworl変換行列を設定　あたいが書かれた後に書く
	pDevice->SetRenderState(D3DRS_LIGHTING, false); //ライト使用

//--------------------------------------------------
// Sceneとendのとこ
//--------------------------------------------------
	pDevice->SetStreamSource(0, g_Vertex, 0, sizeof(MeshVertex));        // 頂点バッファの指定
	pDevice->SetIndices(g_index);    // 頂点インデックスバッファの指定

	pDevice->SetFVF(FVF_MESH);
	pDevice->SetTexture(0, Texture_GetTexture(kannri));

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_vertex_counter, 0, g_primitive_counter);//頂点の数　、　０、三角の数
}


