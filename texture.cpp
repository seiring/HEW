#define _CRT_SECURE_NO_WARNINGS
#include<windows.h>
#include<d3d9.h>
#include<d3dx9.h>
#include"mydirect3d.h"
#include<string.h>


#define FILENAME_MAX (128)
#define TEXTUREDATA_MAX (256)


typedef struct TextureData_tag
{
	char filename[FILENAME_MAX];
	int width;
	int height;
	LPDIRECT3DTEXTURE9 pTexture;
}TextureData;


static LPDIRECT3DDEVICE9 g_pDevice = NULL;
static TextureData g_TextureData[TEXTUREDATA_MAX];


//static TextureData g_TextureData[TEXTUREDATA_MAX] = {};
void Texture_Init(void)  //テクスチャ管理モジュールの初期化
{
	for (int i = 0; i < TEXTUREDATA_MAX; i++)
	{
		g_TextureData[i].filename[0] = 0;
		g_TextureData[i].pTexture = NULL;
		g_TextureData[i].width = 0;
		g_TextureData[i].height = 0;
	}
}

int Texture_SetLoadFile(const char* pFilename, int width, int height)  //ロードしたいファイルを登録しておく ※引数(ファイル名,テクスチャファイルの画像の幅,高さ) 戻り値：管理番号
{
	//すでに登録されていないか？検索
	for (int i = 0; i < TEXTUREDATA_MAX; i++)
	{
		if (strcmp(g_TextureData[i].filename, pFilename) == 0) //0で発見、同じファイルが見つかった
		{
			return i; //管理番号iを返す
		}
	}
	//データベースの使用されていない部分を検索し登録する
	for (int i = 0; i < TEXTUREDATA_MAX; i++)
	{
		if (g_TextureData[i].filename[0] != 0)  //登録されていたら
		{
			continue; //使用済みなので次を検索 
		}
	    //登録処理
		strcpy(g_TextureData[i].filename, pFilename);
		g_TextureData[i].width = width;
		g_TextureData[i].height = height;
		return i;  //管理番号を戻す
	}
	return -1;  //TEXTUREDATA_MAXを使い切った(登録できるところがなかった)
}

int Texture_Load(void)  //今まで登録しておいたファイルがいっきに全て読み込まれる　戻り値：読み込みに失敗した数
{
	g_pDevice = MyDirect3D_GetDevice();

	int error_count = 0; //エラーの数を数える変数

	for (int i = 0; i < TEXTUREDATA_MAX; i++)
	{
		if (g_TextureData[i].filename[0] == 0)  //登録されていなかったら
		{
			continue;
		}
		if (g_TextureData[i].pTexture != 0)
		{
			continue;
		}

		//テクスチャの読み込み
		HRESULT hr = D3DXCreateTextureFromFile(g_pDevice, g_TextureData[i].filename, &g_TextureData[i].pTexture);
		if (FAILED(hr))
		{
			error_count++;
		}
	}
	return error_count;
}

void Texture_Release(int ids[], int count)  //テクスチャを一部開放する関数 引数(int型の配列の先頭アドレス,配列の数) int a[]={1,3,6}
{
	for (int i = 0; i < count; i++)
	{
		if (g_TextureData[ids[i]].pTexture == NULL)
		{
			g_TextureData[ids[i]].pTexture->Release();
			g_TextureData[ids[i]].pTexture = NULL;
			g_TextureData[ids[i]].filename[0] = 0;
			g_TextureData[ids[i]].width = 0;
			g_TextureData[ids[i]].height = 0;
		}
	}
}

void Texture_Release(void) //テクスチャを全開放
{
	for (int i = 0; i < TEXTUREDATA_MAX; i++)
	{
		if (g_TextureData[i].pTexture != NULL)
		{
			g_TextureData[i].pTexture->Release();
			g_TextureData[i].pTexture = NULL;
			g_TextureData[i].filename[0] = 0;
			g_TextureData[i].width = 0;
			g_TextureData[i].height = 0;
		}
	}
}

LPDIRECT3DTEXTURE9 Texture_GetTexture(int id)  //インターフェースポインタが取得できる
{
	return g_TextureData[id].pTexture;
}

int Texture_GetWidth(int id) //テクスチャの幅が取得できる
{
	return g_TextureData[id].width;
} 

int Texture_GetHeight(int id)  //テクスチャの高さが取得できる
{
	return g_TextureData[id].height;
}