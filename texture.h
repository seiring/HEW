#pragma once
#include<d3d9.h>
#include<d3dx9.h>
void Texture_Init(void);  //テクスチャ管理モジュールの初期化
int Texture_SetLoadFile(const char* pFileName, int width, int height);  //ロードしたいファイルを登録しておく ※引数(ファイル名,テクスチャファイルの画像の幅,高さ) 戻り値：管理番号
int Texture_Load(void);  //今まで登録しておいたファイルがいっきに全て読み込まれる　戻り値：読み込みに失敗した数
void Texture_Release(int ids[],int count);
void Texture_Release(void);
LPDIRECT3DTEXTURE9 Texture_GetTexture(int id);
int Texture_GetWidth(int id);
int Texture_GetHeight(int id);