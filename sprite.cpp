#include<windows.h>
#include<d3d9.h>
#include<d3dx9.h>
#include"mydirect3d.h"
#include"texture.h"
#include<stdlib.h>

//頂点の形を表現する頂点構造体を宣言する
typedef struct Vertex2d_tag
{
	D3DXVECTOR4 position;  //座標
	D3DCOLOR color;
	D3DXVECTOR2 uv; //texcoord;
}vertex2d;

//デバイスに頂点の形を伝えるためのFVFを宣言する
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

static D3DCOLOR g_Color = D3DCOLOR_RGBA(255, 255, 255, 255);
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;

void Sprite_SetColor(D3DCOLOR color)
{
	g_Color = color;
}

void Sprite_Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	pDevice->CreateVertexBuffer(sizeof(vertex2d) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);
	pDevice->CreateIndexBuffer(sizeof(WORD) * 6, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIndexBuffer, NULL);
	WORD *pIndex;
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD);
	pIndex[0] = 0,pIndex[1] = 1,pIndex[2] = 2;
	pIndex[3] = 1,pIndex[4] = 3,pIndex[5] = 2;
	g_pIndexBuffer->Unlock();
}

void Sprite_Uninit(void)
{
	if (g_pVertexBuffer)
	{
		g_pVertexBuffer -> Release();
		g_pVertexBuffer = NULL;
	}
}

void Sprite_Draw(int textureId, float dx, float dy)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	if (!pDevice) 
	{ 
		return;
	}  

	int w = Texture_GetWidth(textureId);
	int h = Texture_GetHeight(textureId);

	vertex2d v[] = { {D3DXVECTOR4(dx - 0.5f    , dy - 0.5f    ,0.0f,1.0f), g_Color, D3DXVECTOR2(0.0f,0.0f)},
					 {D3DXVECTOR4(dx - 0.5f + w, dy - 0.5f    ,0.0f,1.0f), g_Color, D3DXVECTOR2(1.0f,0.0f)},
					 {D3DXVECTOR4(dx - 0.5f    , dy - 0.5f + h,0.0f,1.0f), g_Color, D3DXVECTOR2(0.0f,1.0f)},
					 {D3DXVECTOR4(dx - 0.5f + w, dy - 0.5f + h,0.0f,1.0f), g_Color, D3DXVECTOR2(1.0f,1.0f)},
	};

	vertex2d *pv;
	g_pVertexBuffer->Lock(0, 0 ,(void**)&pv,D3DLOCK_DISCARD); 
	//                      ↑全部       ↑仮想アドレス
	memcpy(pv, v, sizeof(v));
	//     ↑先↑元↑量
	g_pVertexBuffer->Unlock(); //Unlockをしたら仮想アドレスは使えない

	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(vertex2d));

	pDevice->SetFVF(FVF_VERTEX2D);  //FVFの設定

	pDevice->SetTexture(0, Texture_GetTexture(textureId));

	pDevice->SetIndices(g_pIndexBuffer);

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

	//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(vertex2d));  //(図形の形,個数,頂点データの先頭アドレス,頂点一個分のサイズ)
}

void Sprite_Draw(int textureId, float dx, float dy,int cut_x,int cut_y,int cut_w,int cut_h)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	if (!pDevice)
	{
		return;
	}

	int w = Texture_GetWidth(textureId);
	int h = Texture_GetHeight(textureId);

	float u0 = cut_x / (float)w;
	float v0 = cut_y / (float)h;
	float u1 = (cut_x + cut_w) / (float)w;
	float v1 = (cut_y + cut_h) / (float)h;


	vertex2d v[] = { {D3DXVECTOR4(dx - 0.5f        , dy - 0.5f ,0.0f   ,1.0f), g_Color, D3DXVECTOR2(u0,v0)},
					 {D3DXVECTOR4(dx - 0.5f + cut_w, dy - 0.5f ,0.0f   ,1.0f), g_Color, D3DXVECTOR2(u1,v0)},
					 {D3DXVECTOR4(dx - 0.5f        , dy - 0.5f + cut_h ,0.0f,1.0f), g_Color, D3DXVECTOR2(u0,v1)},
					 {D3DXVECTOR4(dx - 0.5f + cut_w, dy - 0.5f + cut_h ,0.0f,1.0f), g_Color, D3DXVECTOR2(u1,v1)},
	};

	pDevice->SetFVF(FVF_VERTEX2D);  //FVFの設定

	pDevice->SetTexture(0, Texture_GetTexture(textureId));

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(vertex2d));  //(図形の形,個数,頂点データの先頭アドレス,頂点一個分のサイズ)
}

void Sprite_Draw(int textureId, float dx, float dy, float center_x ,float center_y ,float angle)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	D3DXMATRIX mtxR;
	D3DXMATRIX mtxT;
	D3DXMATRIX mtxIT;

	if (!pDevice) 
	{
		return;
	}

	int w = Texture_GetWidth(textureId);
	int h = Texture_GetHeight(textureId);
	
	//回転行列
	D3DXMatrixRotationZ(&mtxR, angle);
	//平行移動行列
	D3DXMatrixTranslation(&mtxT, -center_x, -center_y, 0.0f);
	                                                   //z
	D3DXMatrixTranslation(&mtxIT, center_x + dx, center_y + dy, 0.0f);

	//行列の合成(合成の順番には意味がある)
	D3DXMATRIX mtxW = mtxT * mtxR * mtxIT;
	                 //平行→回転→平行移動

	vertex2d v[] = { {D3DXVECTOR4(dx - 0.5f     , dy - 0.5f     ,0.0f,1.0f), g_Color, D3DXVECTOR2(0.0f,0.0f)},
					 {D3DXVECTOR4(dx - 0.5f + w , dy - 0.5f     ,0.0f,1.0f), g_Color, D3DXVECTOR2(1.0f,0.0f)},
					 {D3DXVECTOR4(dx - 0.5f     , dy - 0.5f + h ,0.0f,1.0f), g_Color, D3DXVECTOR2(0.0f,1.0f)},
					 {D3DXVECTOR4(dx - 0.5f + w , dy - 0.5f + h ,0.0f,1.0f), g_Color, D3DXVECTOR2(1.0f,1.0f)},
	};

	D3DXVec4Transform(&v[0].position, &v[0].position, &mtxW);
	D3DXVec4Transform(&v[1].position, &v[1].position, &mtxW);
	D3DXVec4Transform(&v[2].position, &v[2].position, &mtxW);
	D3DXVec4Transform(&v[3].position, &v[3].position, &mtxW);

	pDevice->SetFVF(FVF_VERTEX2D);  //FVFの設定

	pDevice->SetTexture(0, Texture_GetTexture(textureId));

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(vertex2d));  //(図形の形,個数,頂点データの先頭アドレス,頂点一個分のサイズ)
}

void Sprite_Draw(int textureId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float center_x, float center_y, float angle)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DXMATRIX mtxR;
	D3DXMATRIX mtxT;
	D3DXMATRIX mtxIT;

	if (!pDevice)
	{
		return;
	}

	int w = Texture_GetWidth(textureId);
	int h = Texture_GetHeight(textureId);

	float u0 = cut_x / (float)w;
	float v0 = cut_y / (float)h;
	float u1 = (cut_x + cut_w) / (float)w;
	float v1 = (cut_y + cut_h) / (float)h;

	//回転行列
	D3DXMatrixRotationZ(&mtxR, angle);
	//平行移動行列
	D3DXMatrixTranslation(&mtxT, -center_x, -center_y, 0.0f);
	//z
	D3DXMatrixTranslation(&mtxIT, center_x + dx, center_y + dy, 0.0f);

	//行列の合成(合成の順番には意味がある)
	D3DXMATRIX mtxW = mtxT * mtxR * mtxIT;
	//平行→回転→平行移動

	vertex2d v[] = { {D3DXVECTOR4(dx - 0.5f        , dy - 0.5f ,0.0f   ,1.0f), g_Color, D3DXVECTOR2(u0,v0)},
					 {D3DXVECTOR4(dx - 0.5f + cut_w, dy - 0.5f ,0.0f   ,1.0f), g_Color, D3DXVECTOR2(u1,v0)},
					 {D3DXVECTOR4(dx - 0.5f        , dy - 0.5f + cut_h ,0.0f,1.0f), g_Color, D3DXVECTOR2(u0,v1)},
					 {D3DXVECTOR4(dx - 0.5f + cut_w, dy - 0.5f + cut_h ,0.0f,1.0f), g_Color, D3DXVECTOR2(u1,v1)},
	};

	WORD index[] = {
		0,1,2,
		1,3,2
	};

	D3DXVec4Transform(&v[0].position, &v[0].position, &mtxW);
	D3DXVec4Transform(&v[1].position, &v[1].position, &mtxW);
	D3DXVec4Transform(&v[2].position, &v[2].position, &mtxW);
	D3DXVec4Transform(&v[3].position, &v[3].position, &mtxW);

	pDevice->SetFVF(FVF_VERTEX2D);  //FVFの設定

	pDevice->SetTexture(0, Texture_GetTexture(textureId));

	//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(vertex2d));  //(図形の形,個数,頂点データの先頭アドレス,頂点一個分のサイズ)

	pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0 , 4 , 2 , index , D3DFMT_INDEX16 , v , sizeof(vertex2d));
}
void Sprite_Draw(int textureId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float center_x, float center_y, float angle,float sx,float sy ,float sz)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DXMATRIX mtxR;
	D3DXMATRIX mtxT;
	D3DXMATRIX mtxIT;
	D3DXMATRIX mtxS;

	if (!pDevice) {
		return;
	}

	int w = Texture_GetWidth(textureId);
	int h = Texture_GetHeight(textureId);

	float u0 = cut_x / (float)w;
	float v0 = cut_y / (float)h;
	float u1 = (cut_x + cut_w) / (float)w;
	float v1 = (cut_y + cut_h) / (float)h;

	//回転行列
	D3DXMatrixRotationZ(&mtxR, angle);
	//平行移動行列
	D3DXMatrixTranslation(&mtxT, -center_x, -center_y, 0.0f);
	//z
	D3DXMatrixTranslation(&mtxIT, center_x + dx, center_y + dy, 0.0f);
	//拡大行列
	D3DXMatrixScaling(&mtxS, sx, sy, sz);

	//行列の合成(合成の順番には意味がある)
	D3DXMATRIX mtxW = mtxT * mtxS * mtxR * mtxIT;
	//平行→回転→平行移動

	vertex2d v[] = { {D3DXVECTOR4(dx - 0.5f        , dy - 0.5f ,0.0f   ,1.0f), g_Color, D3DXVECTOR2(u0,v0)},
					 {D3DXVECTOR4(dx - 0.5f + cut_w, dy - 0.5f ,0.0f   ,1.0f), g_Color, D3DXVECTOR2(u1,v0)},
					 {D3DXVECTOR4(dx - 0.5f        , dy - 0.5f + cut_h ,0.0f,1.0f), g_Color, D3DXVECTOR2(u0,v1)},
					 {D3DXVECTOR4(dx - 0.5f + cut_w, dy - 0.5f + cut_h ,0.0f,1.0f), g_Color, D3DXVECTOR2(u1,v1)},
	};

	WORD index[] = {
		0,1,2,
		1,3,2
	};

	D3DXVec4Transform(&v[0].position, &v[0].position, &mtxW);
	D3DXVec4Transform(&v[1].position, &v[1].position, &mtxW);
	D3DXVec4Transform(&v[2].position, &v[2].position, &mtxW);
	D3DXVec4Transform(&v[3].position, &v[3].position, &mtxW);

	pDevice->SetFVF(FVF_VERTEX2D);  //FVFの設定

	pDevice->SetTexture(0, Texture_GetTexture(textureId));

	//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(vertex2d));  //(図形の形,個数,頂点データの先頭アドレス,頂点一個分のサイズ)

	pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, index, D3DFMT_INDEX16, v, sizeof(vertex2d));
}

void Sprinte_Draw(int textureId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float angle, float sx, float sy, float sz)
{
	D3DCOLOR g_Color = D3DCOLOR_RGBA(255, 255, 255, 255);
	typedef struct Vertex2d_tag
	{
		D3DXVECTOR4 position; //座標
		D3DCOLOR color;
		D3DXVECTOR2 uv; //texcoord;
	}Vertex2d;

	int w = Texture_GetWidth(textureId);
	int h = Texture_GetHeight(textureId);

	float u0 = cut_x / (float)w;
	float v0 = cut_y / (float)h;
	float u1 = (cut_x + cut_w) / (float)w;
	float v1 = (cut_y + cut_h) / (float)h;

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	Vertex2d v[] =
	{
	{ D3DXVECTOR4(dx - 0.5f , dy - 0.5f,0.0f,1.0f),g_Color,D3DXVECTOR2(u0,v0) },//0.5ずらすのはリニアフィルタリングをきれいにするため
	{ D3DXVECTOR4(dx + cut_w - 0.5f , dy - 0.5f,0.0f,1.0f),g_Color,D3DXVECTOR2(u1,v0) },
	{ D3DXVECTOR4(dx - 0.5f , dy + cut_h - 0.5f,0.0f,1.0f),g_Color,D3DXVECTOR2(u0,v1) },
	{ D3DXVECTOR4(dx + cut_w - 0.5f , dy + cut_h - 0.5f,0.0f,1.0f),g_Color,D3DXVECTOR2(u1,v1) },
	};

	//インデックスver１６bit
	WORD index[] =
	{
		0,1,2,
		1,3,2,
	};

	//--------------------------------------
	//原点にど真ん中をセットするためにずらす
	//--------------------------------------
	D3DXMATRIX mtxT;
	D3DXMatrixTranslation(&mtxT, -cut_w / 2 - dx - 0.5f, -cut_h / 2 - dy - 0.5f, 0.0f);
	//--------------------------------------
	//拡大
	//--------------------------------------
	D3DXMATRIX mtxS;
	D3DXMatrixScaling(&mtxS, sx, sy, sz);//1.0fが１００％になってる。半分→０．５ｆ
	//-------------------------------------
	//回転する
	//-------------------------------------
	D3DXMATRIX mtxR;											//変数宣言　floatが十六個
	D3DXMatrixRotationZ(&mtxR, angle);							//出力と回転角（ラジアン）
	//-------------------------------------
	//元に戻す
	//-------------------------------------
	D3DXMATRIX mtxIT;
	D3DXMatrixTranslation(&mtxIT, cut_w / 2 + dx - 0.5f, cut_h / 2 + dy - 0.5f, 0.0f);
	//--------------------------------------
	//合成する
	//--------------------------------------
	D3DXMATRIX mtxW = mtxT * mtxS* mtxR * mtxIT;// 最終的にmtxWで座標変換　　平行移動→回転→平行移動

	D3DXVec4Transform(&v[1].position, &v[1].position, &mtxW); //出力,入力,行列
	D3DXVec4Transform(&v[1].position, &v[1].position, &mtxW); //出力,入力,行列
	D3DXVec4Transform(&v[2].position, &v[2].position, &mtxW); //出力,入力,行列
	D3DXVec4Transform(&v[3].position, &v[3].position, &mtxW); //出力,入力,行列

	if (!pDevice)
	{
		return;
	}

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(textureId));
	pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, index, D3DFMT_INDEX16, v, sizeof(Vertex2d));//インデックス用
}