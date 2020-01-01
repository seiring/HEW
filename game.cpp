//#include"cube.h"
#include"Mesh.h"
#include"cube.h"
#include"scene.h"
#include"input.h"
#include"debug_font.h"
#include"player.h"
#include"wind.h"
#include"uzu.h"
#include"uzumaker.h"
#include"judgement.h"
#include"start.h"
#include"Gorl.h"
#include"sound.h"
#include "Score.h"

static int framecount;
static int framecount2;
void Game_Init()
{
	framecount = 0;
	framecount2 = 0;
	MeshInit(20,20);
	Cube_Init();
	PlayerInit();
	Init_Wind();
	UzuInit();
	UzuMaker_Init();
	InitStart();
	InitGorl();
	PlaySound(SOUND_LABEL_BGM001);
	PlaySound(SOUND_LABEL_BGM002);
}

void Game_Uninit()
{
	//Cube_Uninit();
	UzuUninit();
	//MeshUninit();
	Judgement_Init();
	StopSound(SOUND_LABEL_BGM002);
	StopSound(SOUND_LABEL_BGM001);
}

void Game_Update()
{
	framecount++;
	framecount2++;
	Cube_Update();
	MeshUpdate();
	PlayerUpdate();
	Update_Wind();
	UzuMaker_Update();
	UzuUpdate();
	Judgement_Update();
	if (Keyboard_IsTrigger(DIK_SPACE))
	{
		//変更点ーーーーーーーーーーーーーーーーーーーーーー
		Scene_SetNextScene(SCENE_SCORE);
	}
}

void Game_Draw()
{
	//Cube_Draw(); //波の描画
	MeshDraw();
	Draw_Wind();//風向きの描画
	UzuDraw();  //渦の描画
	PlayerDraw();//プレイヤーの描画
	DrawStart();
	DrawGorl();
}